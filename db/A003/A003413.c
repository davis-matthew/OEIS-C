/*
 * A003413  From a nim-like game.
 * https://oeis.org/A003413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-6) for n >= 8, with a(0..7) = 1, 2, 3, 4, 5,
 *            7, 9, 12
 * Range:     u8 n <= 19, u16 n <= 41, u32 n <= 85, u64 n <= 174,
 *            u128 n <= 350, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static const int init[8] = {1, 2, 3, 4, 5, 7, 9, 12};
    seq_t w[6];
    idx_t i, k;
    if (n < 8) return (seq_t)init[n];
    /* w[k % 6] holds a(k) for the last six indices k */
    for (k = 2; k < 8; k++) w[k % 6] = (seq_t)init[k];
    for (i = 8; i <= n; i++) {
        /* w[i % 6] currently holds a(i-6) */
        w[i % 6] = w[(i - 1) % 6] + w[i % 6];
    }
    return w[n % 6];
}
