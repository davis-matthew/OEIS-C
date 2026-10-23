/*
 * A173599  a(n) = a(n-1) + a(n-2) - [a(n-2)/4] - [a(n-4)/2] - [a(n-6)/4].
 * https://oeis.org/A173599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = a(n-1) + a(n-2) - floor[a(n-2)/4] - floor[a(n-4)/2] -
 *            floor[a(n-6)/4] * a(0) = a(1) = 1 * * Computes values in a
 *            growing static cache so that any order of calls is * correctly
 *            answered and the function stays fast for n up to at least 10 000
 * Range:     u8 n <= 16, u16 n <= 39, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms.
     *   cache[i] holds a(i) for 0 <= i < cache_len.
     */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements currently stored */

    /* Extend cache if we need a larger index. */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp && new_len)        /* allocation failure – give up */
            return (seq_t)0;
        cache = tmp;

        /* Compute new terms from the current end up to and including n. */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0 || i == 1) {
                cache[i] = (seq_t)1;
                continue;
            }

            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a4 = (i >= 4) ? cache[i-4] : (seq_t)0;
            seq_t a6 = (i >= 6) ? cache[i-6] : (seq_t)0;

            seq_t val = a1 + a2;
            val -= a2 / (seq_t)4;   /* floor division, all operands are non‑negative */
            val -= a4 / (seq_t)2;
            val -= a6 / (seq_t)4;

            cache[i] = val;
        }
        cache_len = n + 1;
    }

    /* n is now guaranteed to be within the cache. */
    return cache[n];
}
