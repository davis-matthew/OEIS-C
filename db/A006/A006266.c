/*
 * A006266  A continued cotangent.
 * https://oeis.org/A006266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)^3 + 3*a(n-1), a(0) = 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t x = 2;
    idx_t i;
    for (i = 0; i < n; i++) {
        x = x * x * x + 3 * x;
    }
    return x;
}
