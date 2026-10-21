/*
 * A077242  Combined Diophantine Chebyshev sequences A077240 and A077239.
 * https://oeis.org/A077242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 5, a(1) = 7, a(2) = 23, a(3) = 37,
   a(n) = 6*a(n-2) - a(n-4) for n > 3 */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t  size  = 0;

    if (n >= size) {
        idx_t newsize = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;

        for (idx_t i = size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)5;
            } else if (i == 1) {
                cache[i] = (seq_t)7;
            } else if (i == 2) {
                cache[i] = (seq_t)23;
            } else if (i == 3) {
                cache[i] = (seq_t)37;
            } else {
                cache[i] = cache[i-2] * 6 - cache[i-4];
            }
        }
        size = newsize;
    }
    return cache[n];
}
