/*
 * A121338  Pentagonal numbers P(k) that are one-third of other pentagonal
 *          numbers: P(k) such that 3*P(k)=P(m) for some m>k.
 * https://oeis.org/A121338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7300803*a(n-1) - 7300803*a(n-2) + a(n-3); (a(1)=70,
 *            a(2)=511258935, a(3)=3732600255368600)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allow the function to work for any n >= 1 */
    if (n < 1) return 0;

    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static const seq_t init_vals[3] = {
        (seq_t)70,
        (seq_t)511258935,
        (seq_t)3732600255368600ULL
    };

    /* initialize cache with first three terms */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;              /* allocation failure: give up */
        for (idx_t i = 0; i < 3; ++i)
            cache[i] = init_vals[i];
    }

    /* return already computed value if available */
    if (n <= cache_len)
        return cache[n - 1];

    /* extend cache up to n using the recurrence */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t next = (seq_t)7300803 * cache[i - 1]
                     - (seq_t)7300803 * cache[i - 2]
                     + cache[i - 3];
        /* resize the cache */
        seq_t *tmp = (seq_t*)realloc(cache, (i + 1) * sizeof(seq_t));
        if (!tmp) { free(cache); cache = NULL; cache_len = 0; return 0; }
        cache = tmp;
        cache[i] = next;
    }

    cache_len = n;
    return cache[n - 1];
}
