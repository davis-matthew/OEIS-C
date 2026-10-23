/*
 * A129443  Expansion of (1 - 4*x^2 - 8*x^3)/((1 + 2*x + 4*x^2)*(1 - x - 2*x -
 *          4*x^2 - 4*x^3 + 16*x^4)).
 * https://oeis.org/A129443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = a(n-1) + 6*a(n-2) + 24*a(n-3) + 8*a(n-4) - 16*a(n-5) -
 *            64*a(n-6), with initial values a(0)=1, a(1)=1, a(2)=3, a(3)=25,
 *            a(4)=75, a(5)=289
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   add_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/add_mul.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0, len = 0;

    if (cap == 0)
    {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    if (cap <= n)
    {
        while (cap <= n) cap <<= 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    if (len < 6)
    {
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 3;
        cache[3] = 25;
        cache[4] = 75;
        cache[5] = 289;
        len = 6;
    }

    while (len <= n)
    {
        seq_t val = 0;
        add_mul(&val, cache[len-1], 1, false);   /* a(n-1)          */
        add_mul(&val, cache[len-2], 6, false);   /* +6*a(n-2)       */
        add_mul(&val, cache[len-3], 24, false);  /* +24*a(n-3)      */
        add_mul(&val, cache[len-4], 8, false);   /* +8*a(n-4)       */
        add_mul(&val, cache[len-5], 16, true);  /* -16*a(n-5)      */
        add_mul(&val, cache[len-6], 64, true);  /* -64*a(n-6)      */
        cache[len] = val;
        ++len;
    }

    return cache[n];
}
