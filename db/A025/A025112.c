/*
 * A025112  a(n) = s(1)s(n) + s(2)s(n-1) + ... + s(k)s(n-k+1), where k =
 *          floor(n/2), s = (odd natural numbers).
 * https://oeis.org/A025112
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(4*n^2 - 3*n + 2 + 3*n*(-1)^n)/12
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 2) {
        return 0; /* offset is 2, values below are undefined */
    }
    seq_t nn = (seq_t)n;
    seq_t sign = (n & 1) ? (seq_t)-1 : (seq_t)1;           /* (-1)^n */
    seq_t term = nn * ( (seq_t)4 * nn * nn
                        - (seq_t)3 * nn
                        + (seq_t)2
                        + (seq_t)3 * nn * sign );
    return term / 12;
}
