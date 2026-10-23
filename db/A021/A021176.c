/*
 * A021176  Decimal expansion of 1/172.
 * https://oeis.org/A021176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 172) mod 10 (nth decimal digit of 1/172)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth digit after the decimal point of the reciprocal 1/172. */
#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t rem = 1;   /* remainder of 1/172 after each decimal step */
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i) {
        uint64_t prod = rem * 10ULL;          /* bring down the next decimal place */
        digit = (seq_t)(prod / 172U);         /* current digit (0–9) */
        rem   = prod % 172U;                  /* update remainder for next step */
    }
    return digit;
}
