/*
 * A229226  The partition function G(n,9).
 * https://oeis.org/A229226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{min(9,n)} binomial(n-1,k-1) * a(n-k), a(0) = 1
 *            (OEIS A229226 – partitions of {1,…,n} into blocks of size ≤ 9)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   alloc_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/alloc_cache.h"

static seq_t *cache = NULL;      /* array of computed values (cache[0] = 1) */
static size_t cache_cap = 0;     /* actual allocated size of cache */

/* Ensure that cache has room for indices 0 … n.  Newly allocated
   entries are zero‑initialised.  Any existing data are preserved. */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (!cache) {                       /* first allocation   */
        cache_cap = need > 1 ? need : 1;
        cache = alloc_cache(cache_cap);
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        for (size_t i = 1; i < cache_cap; ++i)
            cache[i] = (seq_t)0;
    }
    else if (cache_cap <= (size_t)n) {  /* enlarge cache      */
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n)
            new_cap = n + 1;
        seq_t *tmp = alloc_cache(new_cap);
        if (!tmp) exit(1);
        for (size_t i = 0; i < cache_cap; ++i)
            tmp[i] = cache[i];
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return a(n) for OEIS A229226.  Results are memoised; the function
   may be called in any order of n. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;               /* a(0) is defined as 1 */

    ensure_cache(n);

    if (cache[n] != (seq_t)0)          /* already computed      */
        return cache[n];

    /* Compute all intermediate values up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue;   /* skip previously known   */
        seq_t sum = (seq_t)0;
        seq_t comb = (seq_t)1;                 /* C(i-1,0) */
        idx_t maxk = i < 9 ? i : 9;
        for (idx_t k = 1; k <= maxk; ++k) {
            /* comb is C(i-1, k-1) */
            sum += comb * cache[i - k];
            if (k < maxk) {
                comb = comb * (seq_t)(i - k); /* C(i-1, k)   */
                comb = comb / (seq_t)k;
            }
        }
        cache[i] = sum;
    }
    return cache[n];
}
