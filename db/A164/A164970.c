/*
 * A164970  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A164970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0..8) =
 *            {1,28,756,20412,551124,14880348,401769396,10847773692,292889889306}
 *            Recurrence for n ≥ 9: a(n) = 26*(a(n-1)+a(n-2)+…+a(n-7)) –
 *            351*a(n-8)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[9] = {
    (seq_t)1,
    (seq_t)28,
    (seq_t)756,
    (seq_t)20412,
    (seq_t)551124,
    (seq_t)14880348,
    (seq_t)401769396,
    (seq_t)10847773692ULL,
    (seq_t)292889889306ULL
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise cache with the first nine terms */
    if (cache_len == 0) {
        cache_len = 9;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (int i = 0; i < 9; ++i) cache[i] = init[i];
    }

    /* return pre‑computed value if available */
    if (n < cache_len) return cache[n];

    /* extend the cache to accommodate up to index n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) { /* allocation failure: fallback to direct computation */
        /* compute iteratively without caching (unlikely to occur) */
        seq_t *tmp_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!tmp_cache) return (seq_t)0;
        for (int i = 0; i < 9; ++i) tmp_cache[i] = init[i];
        for (idx_t i = 9; i <= n; ++i) {
            seq_t sum = 0;
            for (int k = 1; k <= 7; ++k) sum += tmp_cache[i - k];
            tmp_cache[i] = 26 * sum - 351 * tmp_cache[i - 8];
        }
        return tmp_cache[n];
    }
    cache = tmp;

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        for (int k = 1; k <= 7; ++k) sum += cache[i - k];
        cache[i] = 26 * sum - 351 * cache[i - 8];
    }
    cache_len = new_len;
    return cache[n];
}
