/*
 * A006646  Exponential self-convolution of Pell numbers.
 * https://oeis.org/A006646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 4*a(n-2) - 8*a(n-3), a(0) = 0, a(1) = 0, a(2)
 *            = 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t x = 0, y = 0, z = 2; /* a(i-2), a(i-1), a(i) for i = 2 */
    idx_t i;
    if (n < 2) return (seq_t)0;
    for (i = 2; i < n; i++) {
        seq_t t = (seq_t)6 * z - ((seq_t)4 * y + (seq_t)8 * x);
        x = y;
        y = z;
        z = t;
    }
    return z;
}
