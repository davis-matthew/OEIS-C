/*
 * A226222  a(1) = a(2) = a(3) = 1, a(n) = a(n-2-a(n-2)) + a(n-1-a(n-3)) for
 *          n>3.
 * https://oeis.org/A226222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = a(n-2-a(n-2)) + a(n-1-a(n-3)), a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 514, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* 1-based storage, cache[0] unused */
    static size_t cache_len = 0;  /* highest index computed so far */

    if (n == 0) return 0;          /* undefined for n<1, return 0 */

    /* If the value is already computed, return it */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n];

    /* Need to extend the cache to include n */
    size_t new_len = (size_t)n;
    seq_t *p = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
    if (!p)                         /* malloc failure – unlikely in tests */
        return 0;
    cache = p;

    /* Compute new values from (cache_len+1) up to new_len */
    for (size_t k = cache_len + 1; k <= new_len; ++k) {
        if (k <= 3) {
            cache[k] = 1;
        } else {
            /* Compute indices using the recurrence */
            idx_t idx1 = (idx_t)k - 2 - (idx_t)cache[(size_t)(k - 2)];
            idx_t idx2 = (idx_t)k - 1 - (idx_t)cache[(size_t)(k - 3)];
            cache[k] = cache[(size_t)idx1] + cache[(size_t)idx2];
        }
    }
    cache_len = new_len;
    return cache[(size_t)n];
}
