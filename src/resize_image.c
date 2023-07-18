#include <math.h>
#include "image.h"



float nn_interpolate(image im , float x , float y , int c)
{
    int nearest_x = roundf(x);
    int nearest_y = roundf(y);

    return get_pixel(im, nearest_x, nearest_y, c);
}



image nn_resize(image im , int w , int h)
{
    image resized = make_image (w , h , im.c);
    float x_scale = (float) im.w / w;
    float y_scale = (float) im.h / h;

    for (int c = 0 ; c < im.c ; c++)
    {
        for (int y = 0 ; y < h ; y++)
        {
            for (int x = 0 ; x < w ; x++)
            {
                float nearest_x = (x + 0.5) * x_scale - 0.5;
                float nearest_y = (y + 0.5) * y_scale - 0.5;
                float value = nn_interpolate (im , nearest_x , nearest_y , c);
                set_pixel (resized , x , y , c , value);
            }
        }
    }

    return resized; 
    // return make_image(1,1,1);
}



float bilinear_interpolate (image im , float x , float y , int c)
{
    int x_floor = floorf (x);
    int y_floor = floorf (y);
    int x_ceil = ceilf (x);
    int y_ceil = ceilf (y);

    float Q1 = get_pixel (im , x_floor , y_floor , c);
    float Q2 = get_pixel (im , x_ceil , y_floor , c);
    float Q3 = get_pixel (im , x_floor , y_ceil , c);
    float Q4 = get_pixel (im , x_ceil , y_ceil , c);

    float x_weight = x - x_floor;
    float y_weight = y - y_floor;

    float top_interp = Q1 + (Q2 - Q1) * x_weight;
    float bottom_interp = Q3 + (Q4 - Q3) * x_weight;

    return top_interp + (bottom_interp - top_interp) * y_weight;
    
}



image bilinear_resize (image im , int w , int h )
{
    image resized = make_image (w , h , im.c);
    float x_scale = (float) im.w / w;
    float y_scale = (float) im.h / h;

    for (int c = 0 ; c < im.c ; c++)
    {
        for (int y = 0 ; y < h ; y++)
        {
            for (int x = 0 ; x < w ; x++)
            {
                float source_x = (x + 0.5) * x_scale - 0.5;
                float source_y = (y + 0.5) * y_scale - 0.5;

                float value = bilinear_interpolate (im , source_x , source_y , c);
                set_pixel (resized , x , y , c , value);
            }
        }
    }

    return resized;
   // return make_image(1,1,1);
}
