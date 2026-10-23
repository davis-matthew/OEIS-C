/*
 * A170676  Number of reduced words of length n in Coxeter group on 43
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For 1 ≤ n ≤ 48: a(n) = 2 + 41 * Σ_{i=1}^{n} a(n‑i) For
 *            n = 49: a(49) = 1 + 41 * Σ_{i=1}^{48} a(49‑i) For n ≥ 50: a(n) =
 *            41 * Σ_{i=1}^{48} a(n‑i) – 861 * a(n‑49)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache      = NULL;          /* a(0) … a(max_n) */
static idx_t cache_size  = 0;              /* next index to compute */
static size_t cache_cap  = 0;              /* capacity of cache */

static void ensure_cache(idx_t n) {
    if (n < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 128;
    while ((idx_t)new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                    /* allocation failure */
    memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n < cache_size) return cache[n];

    ensure_cache(n);

    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum48 = 0;
        for (idx_t k = 1; k <= 48 && i >= k; ++k)
            sum48 += cache[i - k];

        seq_t val;
        if (i == 0)                     val = 1;
        else if (i <= 48)                val = 2 + 41 * sum48;
        else if (i == 49)                val = 1 + 41 * sum48;
        else                             val = 41 * sum48 - 861 * cache[i - 49];

        cache[i] = val;
    }

    cache_size = n + 1;
    return cache[n];
}
