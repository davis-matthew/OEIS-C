/*
 * A021206  Decimal expansion of 1/202.
 * https://oeis.org/A021206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 202) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const unsigned int MOD = 202;          /* denominator */
    unsigned int rem = 1;                  /* current remainder (r in the long–division algorithm) */
    unsigned int digit;                    /* the digit at the current position */

    for (idx_t i = 0; i <= n; ++i) {
        digit = (rem * 10) / MOD;          /* digit = floor(10 * r / 202) */
        rem   = (rem * 10) % MOD;          /* update remainder: r = (10 * r) mod 202 */
    }

    return (seq_t)digit;                   /* return the nth digit after the decimal point */
}
