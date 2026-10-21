/*
 * A054111  Row sums of array T as in A054110.
 * https://oeis.org/A054111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = (2^(n+4) - 21 + (-1)^(n-1)) / 6 (n ≥ 1)
 *            Recurrence used for efficient computation: a(0) = 1, a(1) = 2
 *            For n ≥ 1: a(n+1) = 2·a(n) + 3 if n is odd a(n+1) = 2·a(n) + 4
 *            if n is even (n in the recurrence is the index of a.)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are provided by the build. */

static seq_t *cache = NULL;     /* Cached a(n) values. */
static size_t  cache_len = 0;   /* Number of entries in cache. */

seq_t a(idx_t n)
{
    size_t i, up_to;

    /* Ensure arrangement for the first two values. */
    if (cache_len == 0) {
        cache_len = 2;                     /* we will store a[0] and a[1] */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;              /* a(0) */
        cache[1] = (seq_t)2;              /* a(1) */
    }
    /* Grow cache if necessary. */
    if ((size_t)n + 1 > cache_len) {
        up_to = (size_t)n;
        if (up_to < 1) up_to = 1;          /* no growth needed for n=0 */
        seq_t *tmp = (seq_t *)realloc(cache, (up_to + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* on allocation failure return 0 */
        cache = tmp;
        /* fill new entries using the recurrence */
        for (i = cache_len; i <= up_to; ++i) {
            seq_t prev = cache[i - 1];
            /* parity of (i-1): odd => add 3, even => add 4 */
            if (((i - 1) & 1) == 1) {
                cache[i] = prev * (seq_t)2 + (seq_t)3;
            } else {
                cache[i] = prev * (seq_t)2 + (seq_t)4;
            }
        }
        cache_len = up_to + 1;
    }
    return cache[(size_t)n];
}
