/*
 * A005021  Random walks (binomial transform of A006054).
 * https://oeis.org/A005021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + a(n-3), a(0) = 1, a(1) = 5, a(2) =
 *            19
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t x = 1, y = 5, z = 19; /* a(i), a(i+1), a(i+2) */
    seq_t t;
    idx_t i;
    if (n == 0) return x;
    if (n == 1) return y;
    for (i = 2; i < n; i++) {
        t = 5 * z + x - 6 * y;
        x = y;
        y = z;
        z = t;
    }
    return z;
}
