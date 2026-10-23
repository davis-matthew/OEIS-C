/*
 * A118195  Self-convolution square-root of A118191, where A118191 is column 0
 *          of the matrix square of triangle A118190 with A118190(n,k) =
 *          (5^k)^(n-k).
 * https://oeis.org/A118195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (S(n) – Σ_{k=1}^{n-1} a(k)·a(n-k)) / 2 where S(n) =
 *            Σ_{k=0}^{n} 5^{k (n-k)} a(0) = 1 This computes the OEIS sequence
 *            A118195 with memoisation
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <cstring>
#endif
#include "../utils/power5.h"

static seq_t *cache       = NULL;   /* stored values a(0)…a(k)   */
static size_t cache_cap   = 0;       /* allocated cache capacity   */
static idx_t  cache_len   = 0;       /* number of values stored    */

/* enlarge the cache so that it can hold at least `new_cap` elements   */
static void grow_cache_to(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;                         /* already large enough */

    if (new_cap < 2)
        new_cap = 2;
    while (cache_cap < new_cap)
        cache_cap = (cache_cap == 0) ? 2 : cache_cap + cache_cap;

    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[cache_cap];
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];
    delete[] cache;
#else
    new_cache = malloc(cache_cap * sizeof(seq_t));
    memcpy(new_cache, cache, cache_len * sizeof(seq_t));
    free(cache);
#endif
    cache = new_cache;
}

/* ensure that cache contains terms up to and including index n   */
static void ensure_cache_upto(idx_t n)
{
    if (n < cache_len)
        return;                         /* already computed */

    grow_cache_to((size_t)n + 1);

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;        /* a(0) = 1 */
            continue;
        }

        /* S(i) = Σ_{k=0}^{i} 5^{k·(i-k)} */
        seq_t S = 0;
        for (idx_t k = 0; k <= i; ++k) {
            idx_t exp = k * (i - k);
            S += power5(exp);
        }

        /* convolution part: Σ_{k=1}^{i-1} a(k)·a(i-k)  (each pair counted twice) */
        seq_t conv = 0;
        for (idx_t k = 1; k < i; ++k)
            conv += cache[k] * cache[i - k];

        /* a(i) = (S - conv) / 2 */
        cache[i] = (S - conv) / (seq_t)2;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache_upto(n);
    return cache[n];
}
