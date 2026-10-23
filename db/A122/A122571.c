/*
 * A122571  a(n) = 14*a(n-1) - a(n-2), with a(1) = a(2) = 1.
 * https://oeis.org/A122571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - a(n-2), a(1)=a(2)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* largest index already computed */

    /* If we already have the answer, just return it */
    if (n <= cache_len)
        return cache[n];

    /* Allocate (or resize) the cache to hold up to index n */
    idx_t required_len = (n > 2) ? n : 2;          /* ensure at least 2 slots */
    seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (required_len + 1));
    if (!tmp && (required_len + 1) > 0)
        return (seq_t)0;   /* allocation failure – unlikely for small n */

    cache = tmp;

    /* Initialise base values if they are missing */
    if (cache_len < 1) {
        cache[1] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len < 2) {
        cache[2] = (seq_t)1;
        cache_len = 2;
    }

    /* Compute missing values up to n using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] * (seq_t)14 - cache[i - 2];
    }
    cache_len = n;

    return cache[n];
}
