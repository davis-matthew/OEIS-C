/*
 * A117746  Numbers of the form k^2 - k - 1 whose digit sum is also a number
 *          of the form k^2 - k - 1.
 * https://oeis.org/A117746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers of the form k^2 - k - 1 whose digit sum is also of that
 *            form
 * Range:     u8 n <= 4, u16 n <= 82, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"   /* digit_sum(seq_t) */
#include "../utils/floor_sqrt.h" /* floor_sqrt(seq_t) */

/* Test whether x = k^2 - k - 1 has a digit sum that can be written as m^2 - m - 1. */
static bool good_candidate(seq_t x)
{
    seq_t s = digit_sum(x);          /* digit sum of x */
    seq_t D = s * 4 + 5;             /* discriminant = 4*s + 5 */
    seq_t t = floor_sqrt(D);         /* floor of sqrt(D) */

    if (t * t != D) return false;    /* discriminant not a perfect square */
    if ((t % 2) == 0) return false;  /* t must be odd to give integer m */

    return true;                     /* m = (t + 1) / 2 is an integer */
}

/* Compute the n-th term of A117746 (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1 */

    /* Static memoisation storage */
    static seq_t *cache = NULL;
    static size_t len   = 0;          /* number of cached terms          */
    static size_t cap   = 0;          /* allocated capacity of cache     */
    static idx_t  next_k = 1;         /* next k to examine (1‑based)     */

    /* Expand cache until it contains at least n terms */
    while (len < (size_t)n)
    {
        seq_t k   = (seq_t)next_k;
        seq_t val = k * k - k - 1;    /* k^2 - k - 1 */

        if (good_candidate(val))
        {
            /* Grow the cache if necessary */
            if (len == cap)
            {
                size_t newcap = (cap == 0) ? 8 : cap * 2;
                seq_t *newarr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!newarr) /* allocation failed – abort gracefully */
                    abort();
                cache = newarr;
                cap   = newcap;
            }
            cache[len++] = val;       /* store the new term */
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}
