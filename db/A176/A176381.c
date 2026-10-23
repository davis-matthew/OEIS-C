/*
 * A176381  y-values in the solution to x^2-71*y^2=1.
 * https://oeis.org/A176381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6960*a(n-1) - a(n-2) with a(1) = 0, a(2) = 413
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache: cache[0] holds a(1), cache[1] holds a(2) */
static seq_t *cache      = NULL;
static idx_t  cache_len  = 0;  /* number of valid entries in cache   */
static idx_t  cache_cap  = 0;  /* allocated capacity of cache        */

seq_t a(idx_t n)
{
    /* 0 is outside the offset – safely return 0 */
    if (n == 0) return 0;

    /* initialise ‑ the first two terms – on the first call */
    if (cache_len == 0) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;         /* a(1) */
        cache[1] = (seq_t)413;       /* a(2) */
        cache_len = 2;
    }

    /* Return a cached value if it already exists */
    if (n <= cache_len) return cache[n-1];

    /* grow the cache if needed */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* iterative recurrence to fill missing entries */
    seq_t prev2 = cache[cache_len-2];
    seq_t prev1 = cache[cache_len-1];
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t cur = (seq_t)6960 * prev1 - prev2;   /* a(i) = 6960*a(i-1)-a(i-2) */
        cache[cache_len] = cur;
        cache_len++;
        prev2 = prev1;
        prev1 = cur;
    }

    return cache[n-1];
}
