/*
 * A068089  Decimal expansion of 104348 / 33215.
 * https://oeis.org/A068089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A068089: decimal digits of 104348 / 33215.
 * a(1) = floor(104348 / 33215) = 3,
 * a(n) for n > 1 = digit n-1 of the fractional part,
 * computed by repeated multiplication of the remainder by 10.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    const idx_t P = 104348;
    const idx_t Q = 33215;

    if (n == 1)
        return (seq_t)(P / Q);

    idx_t rem = P % Q;   /* remainder after integer part */
    /* compute digits after the decimal point */
    for (idx_t i = 1; i < n; ++i) {
        rem *= 10;
        seq_t digit = (seq_t)(rem / Q);
        rem   %= Q;
        if (i == n - 1)
            return digit;
    }
    /* should never reach here for n >= 1 */
    return 0;
}
