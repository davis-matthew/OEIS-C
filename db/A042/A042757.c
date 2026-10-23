/*
 * A042757  Denominators of continued fraction convergents to sqrt(909).
 * https://oeis.org/A042757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Series A042757: Denominators of continued‑fraction convergents to sqrt(909)
   Recurrence: a(0)=1; for n>0
   a(n) = a_coeff(n) * a(n-1) + a(n-2)
   where a_coeff(n) is 30 for n=0, otherwise it cycles through
   [6,1,2,6,2,1,6,60] (period 8). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed denominators */
    static seq_t *cache    = NULL;
    static size_t cache_size = 0;   /* number of terms stored in cache */
    static size_t cache_cap   = 0;   /* allocated capacity */

    /* recurrence state: previous two denominators */
    static seq_t q_prevprev = 1;   /* q_{-2} */
    static seq_t q_prev     = 0;   /* q_{-1} */

    /* coefficients of the continued fraction (period of length 8) */
    static const seq_t period[8] = {6,1,2,6,2,1,6,60};

    /* ensure sufficient memory */
    size_t needed = (size_t)(n + 1);
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* if we have already computed up to n, simply return it */
    if ((idx_t)n < (idx_t)cache_size)
        return cache[(size_t)n];

    /* start computing from the first missing index */
    for (idx_t k = (idx_t)cache_size; k <= n; ++k) {
        seq_t a_k;
        if (k == 0)
            a_k = 30;                      /* a0 = floor(sqrt(909)) */
        else
            a_k = period[(unsigned)((k - 1) % 8)];

        /* recurrence: q_k = a_k * q_{k-1} + q_{k-2} */
        seq_t q_k = a_k * q_prev + q_prevprev;

        /* store the result */
        cache[cache_size] = q_k;
        ++cache_size;

        /* shift state for next iteration */
        q_prevprev = q_prev;
        q_prev     = q_k;
    }

    return cache[(size_t)n];
}
