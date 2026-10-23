/*
 * A164401  Number of binary strings of length n with no substrings equal to
 *          0010 or 0101.
 * https://oeis.org/A164401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) + a(n-6) Initial terms:
 *            a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=14, a(5)=25
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 37, u64 n <= 74, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t len = 0;   /* number of initialized entries */

    /* Ensure we have space for indices 0..n */
    size_t needed = (size_t)n + 1;
    if (needed > capacity) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache) { /* allocation failed – aborting is acceptable */
            /* In a production setting you might handle this differently. */
            abort();
        }
        cache = new_cache;
        capacity = needed;
    }

    /* Compute missing values up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)2;
        } else if (i == 2) {
            cache[2] = (seq_t)4;
        } else if (i == 3) {
            cache[3] = (seq_t)8;
        } else if (i == 4) {
            cache[4] = (seq_t)14;
        } else if (i == 5) {
            cache[5] = (seq_t)25;
        } else {
            seq_t t = 2 * cache[i - 1];
            t -= cache[i - 2];
            t += cache[i - 3];
            t += cache[i - 6];
            cache[i] = t;
        }
    }
    /* Update len to reflect that indices 0..n are now valid */
    len = (size_t)n + 1;
    return cache[n];
}
