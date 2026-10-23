/*
 * A132158  a(3n+k) = 3a(3n+k-1)-3a(3n+k-2)+2a(3n+k-3) for k = 0,1; a(3n+2) =
 *          3a(3n-1)-3a(3n-2), with a(0) = 0,a(1) = 1,a(2) = 3.
 * https://oeis.org/A132158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence:
 *      a(0)=0, a(1)=1, a(2)=3
 *      for n ≥ 3:
 *          if n%3==2      a(n) = 3*a(n-1) - 3*a(n-2)
 *          otherwise      a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3)
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Quick answers for the first three terms. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Grow cache to at least n+1 entries. */
    size_t needed = (size_t)n + 1;
    if (needed > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_len = needed;
    }

    /* Initialise the first three values if not already done. */
    if (cache_len >= 3) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
    } else {
        /* If cache_len < 3 (happens only on first call), set them. */
        if (cache_len >= 1) cache[0] = (seq_t)0;
        if (cache_len >= 2) cache[1] = (seq_t)1;
        if (cache_len >= 3) cache[2] = (seq_t)3;
    }

    /* Compute any missing values up to n. */
    for (size_t i = 3; i <= (size_t)n; ++i) {
        if (i % 3 == 2) {
            cache[i] = 3 * cache[i - 1] - 3 * cache[i - 2];
        } else {
            cache[i] = 3 * cache[i - 1] - 3 * cache[i - 2] + 2 * cache[i - 3];
        }
    }

    return cache[(size_t)n];
}
