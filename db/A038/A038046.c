/*
 * A038046  Shifts left under transform T where Ta is (identity) DCONV a.
 * https://oeis.org/A038046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(1) = 1; a(n > 1) = Sum_{d|(n-1)} d * a((n-1)/d) (OEIS A038046)
 * Range:     u8 n <= 14, u16 n <= 58, u32 n <= 438, u64 n <= 6548,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <new>
#endif

/* The array is indexed from 1.  cache[0] is unused. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* largest index already computed */

/* extend the cache up to n (inclusive) */
static void
ensure_cache(idx_t n)
{
    if (cache_cap >= n)
        return;   /* already enough */

    /* First allocation ---------------------------------------------------- */
    if (!cache) {
        size_t new_cap = (size_t)(n + 1);   /* indices 0..n */
        cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!cache)
            return;   /* out of memory: caller will see 0 */

#ifdef __cplusplus
        /* construct all seq_t objects */
        for (size_t i = 0; i < new_cap; ++i)
            new (&cache[i]) seq_t();
#endif
        /* initialise */
        cache[0] = (seq_t)0;          /* unused slot */
        cache[1] = (seq_t)1;          /* a(1) = 1 */
        cache_cap = 1;
    }
    /* Reallocation --------------------------------------------------------- */
    else if (cache_cap < n) {
        size_t old_cap = cache_cap + 1;          /* allocated size */
        size_t new_cap = (size_t)(n + 1);        /* new size needed */
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache)
            return;   /* out of memory: caller will see 0 */

#ifdef __cplusplus
        /* construct new seq_t objects beyond the previous capacity */
        for (idx_t i = cache_cap + 1; i <= n; ++i)
            new (&cache[i]) seq_t();
#endif
    }

    /* compute missing terms -------------------------------------------- */
    for (idx_t i = cache_cap + 1; i <= n; ++i) {
        idx_t m = i - 1;          /* n-1 in the recurrence */
        seq_t sum = (seq_t)0;
        if (m > 0) {
            for (idx_t d = 1; d <= m / d; ++d) {
                if (m % d == 0) {
                    idx_t e = m / d;
                    sum += (seq_t)d * cache[e];
                    if (e != d)
                        sum += (seq_t)e * cache[d];
                }
            }
        }
        cache[i] = sum;
    }
    cache_cap = n;
}

/* Public function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but safe */

    ensure_cache(n);
    return cache[n];
}
