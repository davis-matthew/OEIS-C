/*
 * A090691  5x - 1 sequence starting at 50 (a(n+1) = a(n)/2 if a(n) is even,
 *          or 5*a(n)-1 if a(n) is odd).
 * https://oeis.org/A090691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 50; a(n+1) = a(n)/2 if a(n) even, else 5*a(n)-1. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1.  For any n < 1 the
       behaviour is unspecified, but we return 0 as a safe value. */
    if (n < 1)
        return (seq_t)0;

    /* Static cache: cache[i] holds a(i).  cache_size is the allocated
       array length (always at least 2, to keep index 1 valid). */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Initialise cache on first call.  We allocate index 0 unused
       and index 1 for a(1). */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(1);           /* out of memory */
        cache[1] = (seq_t)50;           /* a(1) */
    }

    /* Grow the cache if needed. */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;     /* +1 because we use 1‑based index */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);               /* out of memory */
        cache = tmp;
        /* Compute and fill the newly allocated entries. */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            seq_t prev = cache[i - 1];
            /* a is even if the least significant bit is 0. */
            if ((prev & (seq_t)1) != (seq_t)0) {
                cache[i] = prev * (seq_t)5 - (seq_t)1;
            } else {
                cache[i] = prev / (seq_t)2;
            }
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
