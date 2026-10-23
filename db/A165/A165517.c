/*
 * A165517  Indices of the least triangular numbers (A000217) for which three
 *          consecutive triangular numbers sum to a perfect square (A000290).
 * https://oeis.org/A165517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-2) - a(n-4) + 12, with initial values * a(1)=0,
 *            a(2)=5, a(3)=14, a(4)=63. * (The sequence consists of the
 *            indices k such that * (3k^2+9k+8)/2 is a perfect square.)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* highest index computed so far */

    if (n == 0) {  /* invalid, but return 0 to avoid UB */
        return (seq_t)0;
    }

    /* Grow cache if necessary */
    if (n > cache_len) {
        idx_t new_len = n;
        if (!cache) {
            cache = (seq_t*)malloc((new_len + 1) * sizeof(seq_t));
            cache_len = 0;
        } else if (new_len + 1 > cache_len + 1) {
            cache = (seq_t*)realloc(cache, (new_len + 1) * sizeof(seq_t));
        }

        /* Compute missing terms */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)0;
            } else if (i == 2) {
                cache[2] = (seq_t)5;
            } else if (i == 3) {
                cache[3] = (seq_t)14;
            } else if (i == 4) {
                cache[4] = (seq_t)63;
            } else {
                cache[i] = cache[i - 2] * (seq_t)10 - cache[i - 4] + (seq_t)12;
            }
        }
        cache_len = n;
    }

    return cache[n];
}
