/*
 * A029916  Convert n from meters to yards.
 * https://oeis.org/A029916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n * 1250 / 1143)
 * Range:     u8 n <= 0, u16 n <= 9144, u32 n <= 9144, u64 n <= 9144,
 *            u128 n <= 9144, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent integer formula: a(n) = ((seq_t)n * 1250 + 1143/2) / 1143 */

seq_t a(idx_t n)
{
    const seq_t R = 1250;   /* numerator of the fraction          */
    const seq_t B = 1143;   /* denominator of the fraction         */
    return ((seq_t)n * R + (B / 2)) / B;
}
