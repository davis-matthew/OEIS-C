/*
 * A135868  G.f. A(x) = (1 + x*A(2*x))^2.
 * https://oeis.org/A135868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A135868
   G.f. A(x) = (1 + x*A(2*x))^2.
   Recurrence: a(0)=1, a(1)=2,
   a(n) = a(n-1)*2^n + 2^(n-2)*Σ_{k=0}^{n-2} a(k)*a(n-2-k)   (n ≥ 2)   */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation table */
    static seq_t *cache      = NULL;  /* array of a(0)..a(k)   */
    static size_t cache_size = 0;     /* number of elements stored */
    static size_t cache_cap  = 0;     /* allocated capacity       */
    static seq_t pow2;                /* 2^m, where m is next index to compute */

    /* initialise on first call */
    if (!cache) {
        cache_cap  = 16;
        cache      = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0]   = 1;                /* a(0) */
        cache_size = 1;
        pow2       = 2;                /* 2^1 for computing a(1) */
    }

    /* grow table if necessary */
    while (cache_size <= (size_t)n) {
        size_t m = cache_size;        /* compute a(m) */

        /* if capacity exceeded, double it */
        if (cache_size >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* term 1: a(m-1) * 2^m   (pow2 currently equals 2^m) */
        seq_t term1 = cache[m - 1] * pow2;

        /* term 2: 2^(m-2) * Σ_{k=0}^{m-2} a(k)*a(m-2-k)   (if m≥2) */
        seq_t term2 = 0;
        if (m >= 2) {
            seq_t sum = 0;
            for (size_t k = 0; k <= m - 2; ++k) {
                sum += cache[k] * cache[m - 2 - k];
            }
            /* pow2 == 2^m, so pow2 >> 2 == 2^(m-2) */
            seq_t pow2_m_minus2 = pow2 >> 2;
            term2 = sum * pow2_m_minus2;
        }

        /* store a(m) */
        cache[cache_size] = term1 + term2;
        cache_size++;

        /* update pow2 for the next index (2^(m+1)) */
        pow2 = pow2 * 2;
    }

    return cache[(size_t)n];
}
