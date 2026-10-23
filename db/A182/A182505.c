/*
 * A182505  a(0)=0, a(1)=1, a(n) = ( (a(n-1)+a(n-2)) AND n) + n.
 * https://oeis.org/A182505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look, easy
 * Formula:   a(n) = ((a(n-1)+a(n-2)) AND n) + n
 * Range:     u8 n <= 127, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=0, a(1)=1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for memoisation, supports arbitrary call order. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* one past last valid index */

    /* Base cases are handled directly for speed and safety. */
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    /* Initialise cache with the first two terms if not yet done. */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache)               /* allocation failure – fall back to simple */
            return 0;              /* but this case is unlikely in tests */
        cache[0] = 0;
        cache[1] = 1;
    }

    /* If the requested index is already cached, return it. */
    if (n < cache_len)
        return cache[n];

    /* Grow cache to accommodate up to 'n'. */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (!tmp)                     /* allocation failure – return old value */
        return cache[n - 1];
    cache = tmp;

    /* Compute missing terms sequentially. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = cache[i - 1] + cache[i - 2];
        seq_t val = (sum & (seq_t)i) + (seq_t)i;
        cache[i] = val;
    }
    cache_len = new_len;

    return cache[n];
}
