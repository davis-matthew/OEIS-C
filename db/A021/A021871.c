/*
 * A021871  Decimal expansion of 1/867.
 * https://oeis.org/A021871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n-th digit of the decimal expansion of 1/867 (the
 *            sequence of digits after the decimal point)
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t _DEN = 867u;

seq_t a(idx_t n)
{
    seq_t rem = 1;
    seq_t digit = 0;

    /* Compute the first (n+1) digits; the last digit is a(n). */
    for (idx_t i = 0; i <= n; ++i) {
        rem   = rem * 10;         /* shift remainder left by one decimal digit */
        digit = rem / _DEN;       /* current decimal digit */
        rem   = rem % _DEN;       /* new remainder */
    }
    return digit;
}
