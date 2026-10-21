/*
 * A229184  Numbers decremented by their digit product produce a cube.
 * https://oeis.org/A229184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 40, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   digit_product, floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth non‑negative integer m with m - digit_product(m) a perfect cube. */

#include <stdlib.h>
#include "../utils/digit_product.h"
#include "../utils/floor_cube_root.h"

/* External linkage */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cached sequence terms      */
    static size_t cache_size = 0;        /* number of terms cached     */
    static size_t cache_cap  = 0;        /* allocated capacity in cache*/
    static seq_t  next_m     = 0;        /* next integer to test       */

    /* Initial allocation if necessary */
    if (cache == NULL)
    {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache_size = 0;
    }

    /* Generate terms until we have at least n of them */
    while (cache_size < (size_t)n)
    {
        seq_t prod = digit_product(next_m);  /* product of digits of next_m */
        seq_t diff = next_m - prod;          /* the candidate cube value   */

        seq_t k = floor_cube_root(diff);     /* integer cube root of diff */

        /* Check whether diff is a perfect cube */
        seq_t cube = k * k;
        cube = cube * k;
        if (cube == diff)
        {
            /* Ensure capacity for the new term */
            if (cache_size >= cache_cap)
            {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_size++] = next_m;
        }

        next_m++;  /* advance to the next integer */
    }

    /* Return the requested term (1‑based index) */
    return cache[n - 1];
}
