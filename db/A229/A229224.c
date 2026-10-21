/*
 * A229224  The partition function G(n,7).
 * https://oeis.org/A229224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..min(7,n)} C(n‑1, k‑1) * a(n‑k) with a(0)=1
 *            (number of set partitions of {1,…,n} into blocks of size at most
 *            7)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoised values a(0)…a(max_computed) */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static idx_t  cache_len = 0;         /* number of entries currently stored */

/* ensure the cache has space for indices up to 'upto' (inclusive) */
static void ensure_cache(idx_t upto) {
    if ((size_t)upto < cache_cap)
        return;                       /* already enough space */
    size_t new_cap = (cache_cap == 0) ? (size_t)(upto + 1) : cache_cap;
    while (new_cap <= (size_t)upto)
        new_cap <<= 1;                 /* double until large enough */
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_arr;
    cache_cap = new_cap;
}

/* compute the sequence term */
seq_t a(idx_t n) {
    ensure_cache(n);                    /* ensure space for this 'n' */
    if (cache_len <= n) {               /* compute any missing values   */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else {
                seq_t sum = 0;
                idx_t maxk = (i < 7 ? i : 7);
                idx_t m = i - 1;
                for (idx_t k = 1; k <= maxk; ++k) {
                    /* binomial coefficient C(m, k‑1) */
                    seq_t bin = 1;
                    idx_t r = k - 1;
                    for (idx_t j = 1; j <= r; ++j) {
                        bin = bin * (seq_t)(m - j + 1) / (seq_t)j;
                    }
                    sum = sum + bin * cache[i - k];
                }
                cache[i] = sum;
            }
        }
        cache_len = n + 1;              /* all terms up to n are stored */
    }
    return cache[n];
}
