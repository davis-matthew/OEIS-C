/*
 * A041470  Numerators of continued fraction convergents to sqrt(251).
 * https://oeis.org/A041470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* 
 * Formula (recurrence):  p_{-1} = 1,  p_{0} = 15.
 * For k >= 1,  p_k = a_k * p_{k-1} + p_{k-2},
 * where (a_1, a_2, …) are the periodic terms of the continued fraction
 * expansion of sqrt(251).  The sequence a(n) is the numerators of the
 * convergents to sqrt(251), i.e., a(n) = p_n.
 */
seq_t a(idx_t n)
{
    /*-----  continued‑fraction period of sqrt(251)  -----*/
    static uint16_t period[256];
    static size_t period_len = 0;

    if (period_len == 0) {
        const int D = 251;
        int a0 = 0;                      /* floor(sqrt(D)) */
        while ((a0 + 1) * (a0 + 1) <= D) ++a0;

        int m = 0, d = 1, a = a0;
        size_t len = 0;
        for (;;) {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;
            period[len++] = (uint16_t)a;
            if (a == 2 * a0) break;      /* end of the period */
        }
        period_len = len;
    }

    /*-----  cache of already computed numerators  -----*/
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of values stored (>=0) */
    static size_t cache_cap = 0;      /* allocated capacity */

    /* ensure the cache can hold values up to index n (inclusive) */
    if ((size_t)n >= cache_len) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;

        /* allocate or grow the array, taking C++/C into account */
#ifdef __cplusplus
        if (new_cap > cache_cap) {
            seq_t *new_cache = new seq_t[new_cap];
            for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
            delete[] cache;
            cache = new_cache;
            cache_cap = new_cap;
        }
#else
        if (new_cap > cache_cap) {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
#endif

        /* initialise the very first element if the cache was previously empty */
        if (cache_len == 0) {
            cache[0] = (seq_t)15;        /* a0 */
            cache_len = 1;
        }

        /* prepare the two previous numerators for the recurrence */
        seq_t p_prevprev = (cache_len >= 2) ? cache[cache_len - 2] : (seq_t)1;
        seq_t p_prev = (cache_len >= 1) ? cache[cache_len - 1] : (seq_t)15;

        /* compute missing numerators up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t a_i = (i == 0) ? (seq_t)15
                                 : (seq_t)period[(i - 1) % period_len];
            seq_t cur = a_i * p_prev + p_prevprev;
            cache[i] = cur;
            p_prevprev = p_prev;
            p_prev = cur;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
