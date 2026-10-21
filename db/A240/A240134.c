/*
 * A240134  Numerator of (n-1) * ceiling(n/2) / n.
 * https://oeis.org/A240134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(n+3 - (n-1)*(-1)^n)/4
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = n-1 when n even; a(n) = (n-1)*(n+1)/2 when n odd */
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    if ((n & 1) == 0) {               /* n is even */
        return sn - (seq_t)1;
    } else {                          /* n is odd */
        seq_t m = sn - (seq_t)1;
        return m * (sn + (seq_t)1) / (seq_t)2;
    }
}
