/*
 * A118830  2-adic continued fraction of zero, where a(n) = -1 if n is odd,
 *          2*A006519(n/2) otherwise.
 * https://oeis.org/A118830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cofr, sign
 * Formula:   a(n) = -1 if n is odd, 2*A006519(n/2) otherwise
 * Range:     i8 n <= 127, i16 n <= 9548, i32 n <= 9548, i64 n <= 9548,
 *            i128 n <= 9548, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n & 1ULL) {
        return (seq_t)-1;
    }
    idx_t m = n >> 1;                 /* n/2 for even n */
    idx_t lsb = m & -m;               /* lowest power of 2 dividing m */
    return (seq_t)lsb * 2;            /* 2*A006519(m) = 2*lsb */
}
