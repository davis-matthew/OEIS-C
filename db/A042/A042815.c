/*
 * A042815  Denominators of continued fraction convergents to sqrt(938).
 * https://oeis.org/A042815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 34302*a(n-10) - a(n-20), n > 19 Initial terms (0 ≤ n <
 *            20) are obtained from the continued‑fraction expansion of
 *            sqrt(938). The recurrence comes from the period‑10 simple
 *            continued fraction of √938
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute the first len denominators of the convergents of sqrt(938).  */
static void compute_initial_terms(seq_t *arr, size_t len)
{
    const uint64_t D = 938;          /* the square‑root argument          */
    /* integer part of √D */
    uint64_t a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= D) ++a0;

    uint64_t a = a0;    /* a_k at the current step          */
    uint64_t m = 0;     /* m_k                                */
    uint64_t d = 1;     /* d_k                                */

    /* q_{-2} = 1, q_{-1} = 0 (initialised for the recurrence)    */
    seq_t q_prevprev = (seq_t)1;
    seq_t q_prev     = (seq_t)0;
    seq_t q;          /* q_k for the current step           */

    for (size_t k = 0; k < len; ++k) {
        /* q_k = a_k * q_{k-1} + q_{k-2} */
        q = ((seq_t)a) * q_prev + q_prevprev;
        arr[k] = q;

        /* advance to the next partial quotient                */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        /* shift the two previous denominators                */
        q_prevprev = q_prev;
        q_prev     = q;
    }
}

seq_t a(idx_t n)
{
    /* static cache of already computed values               */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise the cache with the first 20 terms             */
    if (cache_len == 0) {
        cache_len = 20;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        compute_initial_terms(cache, cache_len);
    }

    if (n < cache_len) return cache[n];

    /* grow the cache up to n (inclusive) */
    idx_t new_len = n + 1;
    cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 20) continue;                /* already set by init */
        cache[i] = ((seq_t)34302 * cache[i - 10]) - cache[i - 20];
    }
    cache_len = new_len;
    return cache[n];
}
