/*
 * A117833  Triangular numbers for which the product of the digits is a cube.
 * https://oeis.org/A117833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 150, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product, floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Triangular numbers whose digit product is a perfect cube. */
/* The function collects triangular numbers T_k = k*(k+1)/2 and keeps those
   for which the product of the decimal digits of T_k is a perfect cube.
   Results are cached so that calls with arbitrary n are fast. */

#include <stdlib.h>
#include "../utils/digit_product.h"
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* stored triangular numbers that satisfy
                                     the cube‑digit product condition */
    static size_t cache_len = 0;  /* number of terms stored in cache        */
    static size_t cache_cap = 0;  /* allocated capacity of the cache          */
    static idx_t next_k = 0;      /* next triangular index to test           */

    /* Offset is 1, but a(0) is unused; return 0 for safety. */
    if (n == 0)
        return 0;

    /* If we already have the requested term, return it directly. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Generate new terms until we have at least n of them. */
    for (; cache_len < n; ++next_k)
    {
        seq_t t = (seq_t)next_k * ((seq_t)next_k + 1) / 2;          /* T_k */
        seq_t prod = digit_product(t);                              /* ∏digits(T_k) */
        seq_t r = floor_cube_root(prod);                            /* cube root of prod */
        if (r * r * r == prod)                                      /* test cube */
        {
            /* Grow cache if necessary */
            if (cache_len + 1 > cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();  /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = t;  /* store the qualifying triangular number */
        }
    }

    return cache[n - 1];
}
