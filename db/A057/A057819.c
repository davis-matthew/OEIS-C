/*
 * A057819  a(0)=4, a(1)=9, a(n) = 4a(n-1) - a(n-2).
 * https://oeis.org/A057819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n‑1) - a(n‑2), a(0) = 4, a(1) = 9
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of terms cached         */

    /* Initialise cache with first two terms if it is empty. */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)9;
        cache_len = 2;
    }

    /* Return cached value if already computed. */
    if (n < cache_len)
        return cache[n];

    /* Extend cache to hold terms up to index n */
    idx_t new_len = n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i - 1] * 4 - cache[i - 2];
    }

    cache_len = new_len;
    return cache[n];
}
