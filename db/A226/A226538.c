/*
 * A226538  a(2t) = a(2t-1) + 1, a(2t+1) = a(2t) + a(2t-2) for t >= 1, with
 *          a(0) = a(1) = 1.
 * https://oeis.org/A226538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=1; a(2n)=a(2n-1)+1; a(2n+1)=a(2n)+a(2n-2)
 * Range:     u8 n <= 20, u16 n <= 42, u32 n <= 88, u64 n <= 180,
 *            u128 n <= 366, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for memoization, thread‑unsafe but sufficient for the test harness */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* computes a(n) for any n >= 0, memoized in a growing array */
seq_t a(idx_t n)
{
    if (n < cache_cap)        /* already computed */
        return cache[n];

    /* grow the cache until it can hold a[n] */
    size_t needed = (size_t)(n + 1);
    if (cache_cap < needed) {
        size_t new_cap = cache_cap == 0 ? 1 : cache_cap << 1;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory -> terminate */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* fill missing values */
    idx_t start = (cache_cap < (size_t)(n + 1)) ? cache_cap : (idx_t)(n + 1);
    for (idx_t i = 0; i <= n; ++i) {
        if (i == 0 || i == 1)
            cache[i] = 1;
        else if ((i & 1) == 0)             /* i is even, i = 2t */
            cache[i] = cache[i - 1] + 1;
        else                              /* i is odd, i = 2t+1, t>=1 */
            cache[i] = cache[i - 1] + cache[i - 3];
    }
    cache_cap = (size_t)(n + 1);
    return cache[n];
}
