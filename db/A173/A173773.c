/*
 * A173773  a(3*n) = 8*n+2, a(3*n+1) = 2*n+1, a(3*n+2) = 8*n+6.
 * https://oeis.org/A173773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3*n) = 8*n+2, a(3*n+1) = 2*n+1, a(3*n+2) = 8*n+6
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t r = n % 3;
    seq_t q = (seq_t)(n / 3);
    if (r == 0) {
        return (seq_t)8 * q + (seq_t)2;
    } else if (r == 1) {
        return (seq_t)2 * q + (seq_t)1;
    } else {
        return (seq_t)8 * q + (seq_t)6;
    }
}
