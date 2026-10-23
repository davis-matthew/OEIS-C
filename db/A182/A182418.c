/*
 * A182418  a(0)=0, a(n+1) = (a(n) XOR floor(a(n)/6)) + 1, where XOR is the
 *          bitwise exclusive-or operator.
 * https://oeis.org/A182418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(0) = 0; a(n+1) = (a(n) ^ (a(n)/6)) + 1 (recurrence version of
 *            OEIS A182418)
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t may be a builtin unsigned integer type or an
   arbitrary‑precision integer class; idx_t is uint64_t. */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* If the value is already cached, return it */
    if (n < cache_size) {
        return cache[n];
    }

    /* Resize the cache so that it can hold indexes up to n. */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – abort the program */
        abort();
    }
    cache = tmp;

    /* Initialise the base case if this is the first call. */
    if (cache_size == 0) {
        cache[0] = (seq_t)0;   /* a(0) = 0 */
        cache_size = 1;
    }

    /* Compute all the missing terms from cache_size up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        cache[i] = (prev ^ (prev / (seq_t)6)) + (seq_t)1;
    }

    /* Update the cached size. */
    cache_size = n + 1;
    return cache[n];
}
