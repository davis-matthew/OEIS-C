/*
 * A170269  Number of reduced words of length n in Coxeter group on 20
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(0) = 1; for 1 ≤ n ≤ 40: a(n) = 18·∑_{k=0}^{n-1} a(k) +
 *            2; a(41) = 18·∑_{k=0}^{40} a(k) + 1; for n ≥ 42: a(n) =
 *            18·∑_{k=n-40}^{n-1} a(k) – 171·a(n-41);
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache  = NULL;   /* computed terms */
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_len = 0;   /* number of stored terms */

/* enlarge cache so that it contains terms up to and including n */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;

    /* grow array if necessary */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = cache_cap ? (cache_cap << 1) : 16;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *new_p;
#ifdef __cplusplus
        new_p = new seq_t[new_cap];
        for (size_t i = 0; i < cache_len; ++i) new_p[i] = cache[i];
        delete[] cache;
#else
        new_p = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_p) abort();
#endif
        cache = new_p;
        cache_cap = new_cap;
    }

    /* compute all missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        if (i == 0) {
            cache[0] = 1;
        } else if (i <= 40) {
            for (idx_t j = 0; j < i; ++j) sum += cache[j];
            cache[i] = (seq_t)18 * sum + 2;
        } else if (i == 41) {
            for (idx_t j = 0; j < 41; ++j) sum += cache[j];
            cache[i] = (seq_t)18 * sum + 1;
        } else {
            for (idx_t j = i - 40; j < i; ++j) sum += cache[j];
            cache[i] = (seq_t)18 * sum - (seq_t)171 * cache[i - 41];
        }
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
