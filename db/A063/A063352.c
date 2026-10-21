/*
 * A063352  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          79 ).
 * https://oeis.org/A063352
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13 Initial values: a(0)
 *            = 0, a(1) = 0, a(2) = -1, a(3) = 222, a(4) = 481, a(5) = 741,
 *            a(6) = 1001, a(7) = 1261, a(8) = 1521, a(9) = 1781, a(10) =
 *            2041, a(11) = 2301, a(12) = 2561, a(13) = 2819
 * Range:     i8 n <= 2, i16 n <= 41, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* base values for indices 0 … 13 */
static const seq_t base[14] = {
    0,                     /* a(0) */
    0,                     /* a(1) */
   (seq_t)-1,              /* a(2) */
   (seq_t)222,             /* a(3) */
   (seq_t)481,             /* a(4) */
   (seq_t)741,             /* a(5) */
   (seq_t)1001,            /* a(6) */
   (seq_t)1261,            /* a(7) */
   (seq_t)1521,            /* a(8) */
   (seq_t)1781,            /* a(9) */
   (seq_t)2041,            /* a(10) */
   (seq_t)2301,            /* a(11) */
   (seq_t)2561,            /* a(12) */
   (seq_t)2819             /* a(13) */
};

static seq_t *cache = NULL;      /* memoisation array */
static idx_t  cache_len  = 0;     /* allocated size of array     */
static idx_t  max_computed = 1;   /* largest index with a value */

/* Compute a(n) for n >= 2, memoising intermediate values.
 * The function can be called in any order; the array grows as
 * necessary and already‑computed values are reused. */
seq_t a(idx_t n)
{
    /* Values below the offset (n < 2) are irrelevant. */
    if (n < 2) {
        return (seq_t)0;
    }

    /* Directly return the initial values for 2 ≤ n ≤ 13. */
    if (n <= 13) {
        return base[(size_t)n];
    }

    /* Ensure we have room for all indices up to n. */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return zero (should not happen). */
            return (seq_t)0;
        }
        cache = tmp;
        /* Zero‑initialize the new region. */
        for (idx_t i = cache_len; i < new_len; ++i) {
            cache[i] = (seq_t)0;
        }
        cache_len = new_len;
    }

    /* Initialise the lower part of the cache with the known base values
     * the first time we need them.  Do this only once. */
    if (max_computed < 13) {
        for (idx_t i = 0; i <= 13; ++i) {
            cache[i] = base[i];
        }
        max_computed = 13;
    }

    /* Iteratively fill any missing entries up to n. */
    while (max_computed < n) {
        idx_t i = max_computed + 1;
        /* Recurrence applies only for i > 13 – we have already
           initialised all indices ≤ 13. */
        cache[i] = cache[i - 4] + cache[i - 6] - cache[i - 10];
        ++max_computed;
    }

    return cache[n];
}
