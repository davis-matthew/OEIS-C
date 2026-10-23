/*
 * A135247  a(n) = 3*a(n-1) + 2*a(n-2) - 6*a(n-3).
 * https://oeis.org/A135247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) - 6*a(n-3). Initial values: a(0)=1,
 *            a(1)=3, a(2)=11
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation between calls. */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;   /* number of computed terms stored in cache */

    /* Expand cache to accommodate index n. */
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (!tmp && n + 1 > 0)   /* allocation failure (unless we need zero size) */
        abort();             /* signal unrecoverable error */
    cache = tmp;

    /* Initialise base cases if they have not been set yet. */
    if (cached_len == 0 || cached_len <= 1) {
        cache[0] = (seq_t)1;
        cached_len = 1;
    }
    if (cached_len <= 1 && n >= 1) {
        cache[1] = (seq_t)3;
        cached_len = 2;
    }
    if (cached_len <= 2 && n >= 2) {
        cache[2] = (seq_t)11;
        cached_len = 3;
    }

    /* Compute new terms up to n using the recurrence. */
    for (idx_t i = cached_len; i <= n; ++i) {
        seq_t t1 = (seq_t)3 * cache[i - 1];
        seq_t t2 = (seq_t)2 * cache[i - 2];
        seq_t t3 = (seq_t)6 * cache[i - 3];
        cache[i] = t1 + t2 - t3;
    }

    cached_len = n + 1;
    return cache[n];
}
