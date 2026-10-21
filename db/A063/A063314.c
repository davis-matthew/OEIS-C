/*
 * A063314  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          41 ).
 * https://oeis.org/A063314
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 43, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A063314
    Formula: a(n) = a(n-4) + a(n-6) - a(n-10)  (for n > 13)
    Initial values (offset 2):
    a(2) = -1
    a(3) = 51
    a(4) = 120
    a(5) = 190
    a(6) = 260
    a(7) = 330
    a(8) = 400
    a(9) = 470
    a(10) = 540
    a(11) = 610
    a(12) = 680
    a(13) = 748
*/

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache the computed values; static to preserve across calls */
    static seq_t *cache = NULL;      /* array storing a(0), a(1), ... */
    static size_t cache_len = 0;     /* number of values currently in cache */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* We only care about n >= 2. For n < 2 return 0 (undefined but harmless). */
    if (n < (idx_t)2) return 0;

    /* (1) Ensure the cache has space for the desired n. */
    if (n >= (idx_t)cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 32;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }

    /* (2) Pre‑fill the cache with the known initial values if not already done. */
    if (cache_len == 0) {
        /* Initial values for indices 0..13 (ignore 0 and 1 */
        static const seq_t init[14] = {
            0,   /* a(0) placeholder */
            0,   /* a(1) placeholder */
           -1,   /* a(2) */
            51,  /* a(3) */
           120,  /* a(4) */
           190,  /* a(5) */
           260,  /* a(6) */
           330,  /* a(7) */
           400,  /* a(8) */
           470,  /* a(9) */
           540,  /* a(10) */
           610,  /* a(11) */
           680,  /* a(12) */
           748   /* a(13) */
        };
        for (size_t i = 0; i < sizeof(init)/sizeof(init[0]); ++i) {
            cache[i] = init[i];
        }
        cache_len = 14;   /* we now have values up to a(13) */
    }

    /* (3) Compute any missing values up to n using the recurrence. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* Recurrence: a(i) = a(i-4) + a(i-6) - a(i-10) for i > 13 */
        if (i > 13) {
            seq_t val = cache[i-4] + cache[i-6] - cache[i-10];
            cache[i] = val;
        } else {
            /* values for i <= 13 are already in the cache */
        }
    }
    cache_len = (size_t)n + 1;   /* now all indices up to n are valid */

    return cache[n];
}
