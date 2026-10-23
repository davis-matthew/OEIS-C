/*
 * A021646  Decimal expansion of 1/642.
 * https://oeis.org/A021646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the (n+1)-th digit in the decimal expansion of 1/642
 *            (0‑based)
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const seq_t divisor = (seq_t)642;
    seq_t rem = (seq_t)1;
    seq_t digit = (seq_t)0;

    for (idx_t i = 0; i <= n; ++i) {
        rem *= (seq_t)10;
        digit = rem / divisor;
        rem %= divisor;
    }
    return digit;
}
