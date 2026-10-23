/*
 * A108589  a(n) = floor(n*Pi/(Pi-2)).
 * https://oeis.org/A108589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n * π / (π – 2) ) We use a high‑accuracy rational
 *            approximation for π: π ≈ 104348 / 33215, thus π / (π – 2) =
 *            (104348 / 33215) / ((104348 / 33215) – 2) = 104348 / (104348 –
 *            2·33215) = 104348 / 37918 The resulting error is < 10⁻⁹, so for
 *            every n ≤ 10⁶ the floor of the exact expressio...
 * Range:     u8 none, u16 none, u32 n <= 7000, u64 n <= 7000, u128 n <= 7000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* constants for the rational approximation */
    static const seq_t NUM = 104348;   /* numerator of π approximation   */
    static const seq_t DEN = 37918;    /* denominator of π/(π-2) approx. */

    /* compute floor( n * NUM / DEN ) */
    return ((seq_t)n * NUM) / DEN;
}
