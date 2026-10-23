/*
 * A021182  Decimal expansion of 1/178.
 * https://oeis.org/A021182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/178) mod 10 (the nth digit after the
 *            decimal point of 1/178, 0‑based)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* denominator of the rational number */
    const seq_t DEN = (seq_t)178;

    /* current remainder of the division */
    seq_t r = (seq_t)1;

    /* advance the remainder n times to reach the digit's position */
    for (idx_t i = 0; i < n; ++i) {
        r = (r * (seq_t)10) % DEN;
    }

    /* compute the digit at position n */
    seq_t digit = ((r * (seq_t)10) / DEN);
    return digit;
}
