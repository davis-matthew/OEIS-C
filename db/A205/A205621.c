/*
 * A205621  Number of (n+1) X 4 0..2 arrays with every 2 X 2 subblock in a row
 *          having an equal number of equal diagonal or equal antidiagonal
 *          elements, adjacent rows differing in this number, and new values
 *          0..2 introduced in row major order.
 * https://oeis.org/A205621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 72*a(n-2) + 48*a(n-3) - 112*a(n-4) for n>5; a(1)=194,
 *            a(2)=1512, a(3)=13068, a(4)=113592, a(5)=992088
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises the sequence in a static array.
   It computes terms up to the requested index using the recurrence
   and returns the value. It works for any integer type seq_t,
   provided seq_t supports the arithmetic operations used here. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache: cache[1] holds a(1), cache[0] unused */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    /* initialise cache with the first 5 terms */
    if (capacity == 0) {
        capacity = 6;   /* 0..5 indices */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */
        cache[1] = (seq_t)194;
        cache[2] = (seq_t)1512;
        cache[3] = (seq_t)13068;
        cache[4] = (seq_t)113592;
        cache[5] = (seq_t)992088;
    }

    if (n < 1) return (seq_t)0;   /* undefined for n<1 */

    /* extend cache if necessary */
    if ((size_t)n >= capacity) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* allocation failure */
        /* compute new terms using recurrence */
        for (idx_t i = capacity; i <= n; ++i) {
            /* For i<=5 we already have the initial values */
            if (i <= 5) {
                new_cache[i] = cache[i];
                continue;
            }
            /* a(i) = 72*a(i-2) + 48*a(i-3) - 112*a(i-4) */
            new_cache[i] = (seq_t)72 * new_cache[i-2]
                         + (seq_t)48 * new_cache[i-3]
                         - (seq_t)112 * new_cache[i-4];
        }
        cache = new_cache;
        capacity = new_cap;
    }

    return cache[(size_t)n];
}
