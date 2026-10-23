/*
 * A170493  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1
   for 1 ≤ n ≤ 45: a(n) = 2 + 2 * Σ_{j=0}^{n-1} a(j)
   for n ≥ 46:     a(n) = 2 * Σ_{k=n-45}^{n-1} a(k) - 3 * a(n-46)  */
/* This implements the linear recurrence derived from the rational
   generating function of A170493.                              */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;   /* array of values */
    static size_t  cap    = 0;      /* current capacity */
    static size_t  len    = 0;      /* number of values stored   */

    /* initialisation */
    if (cache == NULL) {
        cap = 64;                                   /* start with room for the first terms */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;                       /* hard‑fallback (should never happen) */
        cache[0] = (seq_t)1;                        /* a(0) = 1 */
        len = 1;
    }

    /* grow the array if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while (new_cap < (size_t)n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        cap   = new_cap;
    }

    /* if the requested index is already computed, return it */
    if ((size_t)n + 1 <= len)
        return cache[(size_t)n];

    /* prefix sum of all already computed terms – used for  n < 46  */
    seq_t prefix = 0;
    for (size_t i = 0; i < len; ++i)
        prefix += cache[i];

    /* sliding window sum of the last 45 terms – used for n ≥ 46 */
    seq_t sum_window = 0;
    if (len >= 46) {
        for (size_t i = len - 45; i < len; ++i)
            sum_window += cache[i];
    }

    /* compute missing terms up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t val;

        if (i <= 45) {                     /* first 46 terms */
            if (i == 0)
                val = (seq_t)1;            /* a(0) */
            else
                val = (seq_t)2 + (seq_t)2 * prefix;  /* a(n)=2+2*sum_{j< n} a(j) */
            cache[i] = val;
            prefix += val;
        } else {                           /* n ≥ 46 : use recurrence */
            if (i == 46) {                 /* initialise window */
                sum_window = 0;
                for (size_t k = 1; k <= 45; ++k)
                    sum_window += cache[k];
            }
            val = (seq_t)2 * sum_window - (seq_t)3 * cache[i - 46];
            cache[i] = val;

            /* update sliding window for the next step */
            sum_window += val;
            if (i >= 45)                   /* remove the element that leaves the window */
                sum_window -= cache[i - 45];
        }
    }

    len = (size_t)n + 1;                   /* cache now contains all values up to n */
    return cache[(size_t)n];
}
