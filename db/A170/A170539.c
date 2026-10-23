/*
 * A170539  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^46 = I.
 * https://oeis.org/A170539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for 1 ≤ n ≤ 45, a(n)=50*49^(n‑1); for n≥46,
 *            a(n)=48*a(n‑1)+48*a(n‑2)+…+48*a(n‑45)-1176*a(n‑46)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* persistent cache of computed terms */
    static seq_t *cache = NULL;          /* array of a[0..] */
    static size_t cache_len = 0;          /* allocated size */
    static size_t last_computed = 0;      /* index of largest computed term */

    /* for the 49^(k) factors used up to n=45 */
    static seq_t pow49 = (seq_t)1;        /* holds 49^(k) */
    static size_t powk   = 0;             /* current exponent (k) */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_len = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) abort();
        cache[0] = (seq_t)1;             /* a(0) */
        last_computed = 0;
    }

    if (n == 0) return cache[0];

    /* grow cache if needed */
    if ((size_t)n >= cache_len) {
        size_t new_len = cache_len * 2;
        while (new_len <= (size_t)n) new_len *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!new_cache) abort();
        cache = new_cache;
        cache_len = new_len;
    }

    /* compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 45) {
            /* a(i) = 50 * 49^(i-1)  (special case i=1 gives 50) */
            if (i == 1) {
                val = (seq_t)50;
            } else {
                val = (seq_t)50 * pow49;
            }
            pow49 *= (seq_t)49;        /* prepare for next exponent */
        } else {
            /* recurrence a(i)=48*sum_{k=1}^{45} a(i-k) - 1176*a(i-46) */
            seq_t sum45 = (seq_t)0;
            for (size_t k = 1; k <= 45; ++k) {
                sum45 += cache[i - k];
            }
            val = (seq_t)48 * sum45 - (seq_t)1176 * cache[i - 46];
        }
        cache[i] = val;
    }

    last_computed = (size_t)n;
    return cache[n];
}
