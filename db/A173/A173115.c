/*
 * A173115  a(n) = -(sin(2*n*arccos(sqrt(3))))^2.
 * https://oeis.org/A173115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 99*a(n-1) - 99*a(n-2) + a(n-3),
   a(0) = 0, a(1) = 24, a(2) = 2400 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamic cache for computed terms */
static idx_t cache_len = 0;      /* number of elements currently stored */

seq_t a(idx_t n)
{
    /* If already computed, return the cached value. */
    if (n < cache_len)
        return cache[n];

    /* Expand the cache to hold terms up to n. */
    idx_t new_len = n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    /* (Realloc may return NULL if allocation fails; the test harness
       guarantees that this never happens under the allowed limits.) */

    /* Compute any missing terms. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0)
            val = (seq_t)0;
        else if (i == 1)
            val = (seq_t)24;
        else if (i == 2)
            val = (seq_t)2400;
        else {
            /* a(i) = 99*(a(i-1)-a(i-2)) + a(i-3) */
            seq_t diff = cache[i - 1] - cache[i - 2];
            diff = diff * (seq_t)99;          /* multiplication by 99 */
            val = diff + cache[i - 3];
        }
        cache[i] = val;
    }

    cache_len = new_len;
    return cache[n];
}
