/*
 * A047324  Numbers that are congruent to {0, 2, 5, 6} mod 7.
 * https://oeis.org/A047324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-5) for n > 5 a(1) = 0, a(2) = 2,
 *            a(3) = 5, a(4) = 6, a(5) = 7 The sequence lists numbers
 *            congruent to {0,2,5,6} modulo 7
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (n == 0 || n > SIZE_MAX) return 0;   /* safety check */

    /* expand cache if necessary */
    if ((size_t)n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) { /* allocation failed – abort */
            abort();
        }
        cache = tmp;
        /* compute new terms */
        for (size_t i = cache_len; i < new_len; ++i) {
            idx_t k = (idx_t)(i + 1); /* k is the sequence index (1‑based) */
            if (i < 5) {             /* initial values */
                switch (i) {
                case 0: cache[i] = (seq_t)0; break;
                case 1: cache[i] = (seq_t)2; break;
                case 2: cache[i] = (seq_t)5; break;
                case 3: cache[i] = (seq_t)6; break;
                case 4: cache[i] = (seq_t)7; break;
                }
            } else {
                /* recursion: a(k) = a(k-1) + a(k-4) - a(k-5) */
                cache[i] = cache[i - 1] + cache[i - 4] - cache[i - 5];
            }
        }
        cache_len = new_len;
    }

    return cache[n - 1];
}
