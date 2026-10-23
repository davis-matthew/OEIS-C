/*
 * A017954  Expansion of 1/((1-3x)*(1-6x)*(1-12x)).
 * https://oeis.org/A017954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=21, a(2)=315; for n>2, a(n) = 21*a(n-1) -126*a(n-2)
 *            +216*a(n-3)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    const seq_t a0 = 1;
    const seq_t a1 = 21;
    const seq_t a2 = 315;

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;

    seq_t prev3 = a0, prev2 = a1, prev1 = a2;
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        next = prev1 * 21 - prev2 * 126 + prev3 * 216;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
