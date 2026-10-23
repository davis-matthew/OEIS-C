/*
 * A022131  Fibonacci sequence beginning 4, 11.
 * https://oeis.org/A022131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 4, a(1) = 11
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;

    /* Initialise with first two terms if not yet done */
    if (len == 0) {
        cap = 4; /* arbitrary initial capacity */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = 4;
        cache[1] = 11;
        len = 2;
    }

    /* Extend cache up to requested index */
    while (len <= n) {
        if (len >= cap) {
            cap <<= 1; /* double capacity */
            cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
        }
        cache[len] = cache[len-1] + cache[len-2];
        len++;
    }

    return cache[n];
}
