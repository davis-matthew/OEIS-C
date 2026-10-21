/*
 * A231518  Number of nX3 0..1 arrays with no element less than a strict
 *          majority of its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A231518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence (order 13)
   a(n) = 4*a(n-1) - 2*a(n-2) + 43*a(n-3) - 50*a(n-4) + 33*a(n-5)
          - 307*a(n-6) + 115*a(n-7) - 66*a(n-8) + 490*a(n-9) + 60*a(n-10)
          - 144*a(n-11) + 32*a(n-12) - 192*a(n-13)   for n > 14
   Initial values (n = 1 .. 14):
   4, 34, 153, 776, 3861, 18721, 91993, 453274, 2223662,
   10915727, 53635324, 263420767, 1293604417, 6353594215 */

#include <stdlib.h>

/* Table of initial values */
static const seq_t init_vals[14] = {
    (seq_t)4,
    (seq_t)34,
    (seq_t)153,
    (seq_t)776,
    (seq_t)3861,
    (seq_t)18721,
    (seq_t)91993,
    (seq_t)453274,
    (seq_t)2223662,
    (seq_t)10915727,
    (seq_t)53635324,
    (seq_t)263420767,
    (seq_t)1293604417,
    (seq_t)6353594215
};

/* Coefficients for the recurrence, from a(n-1) to a(n-13) */
static const int coeffs[13] = {
    4,   -2,  43, -50, 33,
  -307, 115, -66, 490, 60,
 -144, 32, -192
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* allocated size (indices 0..cache_size-1) */
    static size_t current_max = 0;     /* largest index computed so far */

    /* Grow the cache if necessary (C++ cast required) */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;          /* allocation failure */
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute missing entries up to n */
    for (size_t i = current_max + 1; i <= (size_t)n; ++i) {
        if (i <= 14) {
            cache[i] = init_vals[i - 1];
        } else {
            seq_t pos = 0, neg = 0;
            for (int k = 0; k < 13; ++k) {
                seq_t val = cache[(size_t)i - 1 - (size_t)k];
                int c = coeffs[k];
                if (c >= 0)
                    pos += (seq_t)c * val;
                else
                    neg += (seq_t)(-c) * val;
            }
            cache[i] = pos - neg;
        }
    }
    current_max = (size_t)n;
    return cache[n];
}
