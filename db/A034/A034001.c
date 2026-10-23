/*
 * A034001  One third of triple factorial numbers.
 * https://oeis.org/A034001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^(n-1) * n! (a(1)=1, a(n)=3*n*a(n-1) for n>=2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* out of offset, value irrelevant */

    static seq_t *cache = NULL;           /* cache[0] holds a(1) */
    static idx_t cache_len = 0;           /* maximum n that has been stored */
    static size_t cache_cap = 0;          /* allocated elements */

    /* Allocate initial cache entry for a(1) if needed */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failure: abort */
        cache[0] = (seq_t)1;             /* a(1) = 1 */
        cache_len = 1;
    }

    /* Extend cache capacity if necessary */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failure: abort */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n using the recurrence */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        seq_t prev = cache[k - 2];       /* a(k-1) is already computed */
        seq_t prod = (seq_t)3 * (seq_t)k; /* 3*k  */
        cache[k - 1] = prod * prev;     /* a(k) = 3*k*a(k-1) */
    }

    cache_len = n;
    return cache[n - 1];
}
