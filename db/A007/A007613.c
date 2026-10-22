/*
 * A007613  a(n) = (8^n + 2*(-1)^n)/3.
 * https://oeis.org/A007613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (8^n + 2*(-1)^n)/3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   a000272_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000272_ipow.h"

seq_t a(idx_t n) {
    seq_t power_8 = a000272_ipow((seq_t)8, n);
    if (n % 2 == 0) {
        return (power_8 + 2) / 3;
    } else {
        return (power_8 - 2) / 3;
    }
}
