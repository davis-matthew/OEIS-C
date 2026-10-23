/*
 * A164598  a(n) = 12*a(n-1) - 34*a(n-2), for n > 1, with a(0) = 1, a(1) = 14.
 * https://oeis.org/A164598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 34*a(n-2), with a(0) = 1, a(1) = 14
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Static cache for memoization; zero-initialized by static storage duration */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of terms currently filled */

    /* Ensure the cache has room for up to n+1 elements. */
    if ((size_t)n + 1 > cache_cap) {
        grow_seq(&cache, &cache_cap, (size_t)n + 1);
    }

    /* Initialise the first two terms if needed. */
    if (cache_len < 2) {
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)14;  /* a(1) */
        cache_len = 2;
    }

    /* Compute any terms that haven't been cached yet. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t t1 = cache[i - 1] * (seq_t)12;
        seq_t t2 = cache[i - 2] * (seq_t)34;
        cache[i] = t1 - t2;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
