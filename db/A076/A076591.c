/*
 * A076591  a(1)=1, a(2)=2 a(n)=a(n-1)+(a(n-2) mod n).
 * https://oeis.org/A076591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2, a(n) = a(n-1) + (a(n-2) mod n) for n > 2
 * Range:     u8 n <= 29, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of computed terms.  cache_len holds the largest
       index that has been computed and stored in cache[1..cache_len].
       cache[0] is unused to keep indices natural. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Ensure base terms a(1) and a(2) are present. */
    if (cache_len < 2) {
        cache = (seq_t *)realloc(cache, 3 * sizeof(seq_t)); /* indices 0,1,2 */
        cache[1] = 1;
        cache[2] = 2;
        cache_len = 2;
    }

    /* Return immediately if the term has already been computed. */
    if (n <= cache_len)
        return cache[n];

    /* Extend the cache up to index n. */
    cache = (seq_t *)realloc(cache, (size_t)(n + 2) * sizeof(seq_t)); /* +2 for safe indexing */

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t rem = cache[i - 2] % (seq_t)i;
        cache[i] = cache[i - 1] + rem;
    }
    cache_len = n;
    return cache[n];
}
