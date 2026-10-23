/*
 * A166369  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^11 = I.
 * https://oeis.org/A166369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*∑_{j=1}^{10} a(n‑j) – 45*a(n‑11) with the special case
 *            a(11) = 9*∑_{j=1}^{10} a(11‑j) – 45*a(0) + 1 (this +1 comes from
 *            the numerator of the generating function). Initial values:
 *            a(0)=1, a(1)=11, a(k)=a(k‑1)*10 for k=2…10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_used = 0;      /* number of stored elements (max index+1) */

    /* ---- initialise first 11 terms if needed ---- */
    if (cache_used == 0) {
        size_t init_cap = 32;          /* small initial capacity */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure – return 0 */

        cache_cap = init_cap;
        cache[0] = (seq_t)1;          /* a(0) */
        cache[1] = (seq_t)11;         /* a(1) */
        for (idx_t i = 2; i <= 10; ++i) {
            cache[i] = cache[i-1] * (seq_t)10;  /* a(i) = a(i-1)*10 */
        }
        cache_used = 11;              /* indices 0 … 10 present */
    }

    /* ---- grow the cache if the requested index is beyond current capacity ---- */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 32;
        while ((size_t)n + 1 > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure – return 0 */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* ---- immediately return cached value if available ---- */
    if ((size_t)n < cache_used) {
        return cache[n];
    }

    /* ---- compute a(11) with the special constant +1 if needed ---- */
    if (cache_used <= 11 && n >= 11) {
        /* compute sum of a(1) … a(10) */
        seq_t sum = 0;
        for (idx_t j = 1; j <= 10; ++j) {
            sum += cache[j];
        }
        /* a(11) = 9*sum – 45*a(0) + 1 */
        seq_t val = sum * (seq_t)9 - cache[0] * (seq_t)45 + (seq_t)1;
        cache[11] = val;
        cache_used = 12;              /* indices 0 … 11 present */
        if ((size_t)n == 11) {
            return val;
        }
    }

    /* ---- compute terms from the current index up to n ---- */
    idx_t current = cache_used;      /* first not yet computed index (>=12) */

    /* Initialise sliding window sum of the last 10 terms before 'current' */
    seq_t sum = 0;
    for (idx_t j = current - 10; j < current; ++j) {
        sum += cache[j];
    }

    for (idx_t i = current; i <= n; ++i) {
        /* recurrence for n >= 12 */
        seq_t val = sum * (seq_t)9 - cache[i-11] * (seq_t)45;
        cache[i] = val;

        /* update sliding window sum for the next index */
        if (i < n) {
            sum -= cache[i-10];
            sum += cache[i];
        }
    }

    cache_used = (size_t)n + 1;
    return cache[n];
}
