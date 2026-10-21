/*
 * A079965  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=1, r=5, I={0,3}.
 * https://oeis.org/A079965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2) + a(n-3) + a(n-5) + a(n-6)
   with initial values a(0)=1, a(1)=0, a(2)=1, a(3)=1, a(4)=1, a(5)=3. */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0U;   /* number of elements currently stored */

    /* if we already have the value, return it immediately */
    if (n < cache_size)
        return cache[n];

    /* grow the cache to hold indices 0 … n (inclusive) */
    idx_t new_size = n + 1U;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        abort();            /* out‑of‑memory */

    /* compute new entries starting from the first missing index */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        if (i == 0U)
            new_cache[i] = 1U;
        else if (i == 1U)
            new_cache[i] = 0U;
        else if (i == 2U)
            new_cache[i] = 1U;
        else if (i == 3U)
            new_cache[i] = 1U;
        else if (i == 4U)
            new_cache[i] = 1U;
        else if (i == 5U)
            new_cache[i] = 3U;
        else
            new_cache[i] = new_cache[i - 2U]
                         + new_cache[i - 3U]
                         + new_cache[i - 5U]
                         + new_cache[i - 6U];
    }

    /* install the enlarged cache for future calls */
    cache = new_cache;
    cache_size = new_size;

    return cache[n];
}
