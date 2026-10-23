/*
 * A042813  Denominators of continued fraction convergents to sqrt(937).
 * https://oeis.org/A042813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of convergents to sqrt(937) in the continued fraction expansion */

#include <stdlib.h>
#include <stdint.h>

/* Function: a(n)
 *  Returns the nth denominator of the convergents to sqrt(937).
 *  The sequence is indexed starting at n = 0, where a(0) = 1. */
seq_t a(idx_t n)
{
    /* static state for memoization across calls */
    static seq_t *cache      = NULL;   /* dynamic array of terms */
    static idx_t cache_size  = 0;      /* number of terms stored */
    static idx_t cache_cap   = 0;      /* current capacity of cache array */

    /* continuants of sqrt(937) */
    static int64_t cf_m       = 0;     /* m_k of the CF algorithm */
    static int64_t cf_d       = 1;     /* d_k */
    static int64_t cf_a       = 30;    /* next partial quotient a_k (initial a0) */
    static const int64_t a0   = 30;    /* floor(sqrt(937)) */
    static const int64_t D    = 937;   /* the number under the square root */

    /* previous two denominator terms: q_{k-2} and q_{k-1} */
    static seq_t q_prev2 = 0;          /* q_{-1} */
    static seq_t q_prev1 = 1;          /* q_{0} */

    /* initialise cache and CF state only on the first call */
    if (cache_size == 0) {
        /* give the cache an initial capacity */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – return 0 as sentinel */

        cache[0] = (seq_t)1;          /* a(0) */
        cache_size = 1;
    }

    /* grow the cache if the requested index is beyond what we have */
    if (cache_size <= n) {
        /* compute terms until we reach index n */
        while (cache_size <= n) {
            /* generate the next partial quotient a_k (k ≥ 1) */
            cf_m = cf_d * cf_a - cf_m;
            cf_d = (D - cf_m * cf_m) / cf_d;
            cf_a = (a0 + cf_m) / cf_d;      /* floor division */

            /* compute the next denominator using the recurrence:
               q_k = a_k * q_{k-1} + q_{k-2} */
            seq_t new_q = (seq_t)cf_a * q_prev1 + q_prev2;

            /* ensure the cache array is large enough */
            if (cache_cap <= cache_size) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                if (new_cap < cache_size + 1) new_cap = cache_size + 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return cache[cache_size - 1]; /* allocation failure – return last known term */
                cache = tmp;
                cache_cap = new_cap;
            }

            cache[cache_size] = new_q;
            cache_size++;

            /* shift the previous denominators for next iteration */
            q_prev2 = q_prev1;
            q_prev1 = new_q;
        }
    }

    return cache[(size_t)n];
}
