/*
 * A072481  a(n) = Sum_{k=1..n} Sum_{d=1..k} (k mod d).
 * https://oeis.org/A072481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1..n} ∑_{d=1..k} (k mod d)
 * Range:     u8 n <= 16, u16 n <= 103, u32 n <= 4171, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A072481.
 *
 * The function keeps an incremental cache so that arbitrary calls to a(n)
 * with any n ≥ 0 are answered in O(√n) amortised time and O(max_n)
 * memory.  The inner sum ∑_{d=1..k} (k mod d) is evaluated in
 * time O(√k) using a classic divisor‑summation trick:
 *
 *   ∑_{d=1..k} (k mod d) = k² − ∑_{d=1..k} d · ⌊k/d⌋
 *
 * While the outer sum is computed incrementally so that each new
 * term is produced by adding the inner sum for the next k to the
 * previously cached cumulative sum.
 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache state */
    static seq_t *cache      = NULL;   /* cache[0] = 0, cache[k] = a(k) */
    static idx_t  cache_len  = 0;      /* number of elements currently stored (>= computed index + 1) */
    static idx_t  computed   = 0;      /* largest n for which a(k) is cached */

    if (n == 0) return 0;              /* a(0) = 0 */

    /* initialise cache on first call */
    if (computed == 0) {
        cache_len = 2;                 /* allocate space for indices 0 and 1 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return 0;         /* out of memory – unlikely in test harness */
        cache[0] = 0;
        cache[1] = 0;                 /* a(1) is 0, will be corrected when computing for k = 1 */
        computed = 1;
    }

    /* grow cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = cache_len;
        while (new_len <= n) new_len <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) return 0;           /* out of memory */
        cache = tmp;
        cache_len = new_len;
    }

    /* compute terms for k from computed+1 up to n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        /* inner sum S(k) = ∑_{d=1..k} (k mod d) */
        seq_t k_sq = (seq_t)k * (seq_t)k;        /* k² */
        seq_t sub = 0;                           /* will accumulate Σ d·floor(k/d) */
        idx_t i = 1;
        while (i <= k) {
            idx_t q = k / i;                     /* floor(k/d) for current d */
            idx_t last = k / q;                  /* last d with same floor value */
            if (last > k) last = k;
            /* sum of integers from i to last: (i + last) * (last - i + 1) / 2 */
            seq_t sum_d = (seq_t)(i + last) * (seq_t)(last - i + 1) / 2;
            sub += (seq_t)q * sum_d;
            i = last + 1;
        }
        seq_t inner = k_sq - sub;                 /* S(k) */
        cache[k] = cache[k - 1] + inner;          /* cumulative sum a(k) */
    }

    computed = n;
    return cache[n];
}
