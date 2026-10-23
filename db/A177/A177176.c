/*
 * A177176  Partial sums of round(n^2/13).
 * https://oeis.org/A177176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n*(n+1)*(2*n+1)/78) = (n*(n+1)*(2*n+1) + 39) / 78
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1747, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = nn * (nn + 1) * (seq_t)(2 * n + 1); /* numerator of the fraction */
    return (term + 39) / 78;                         /* integer rounding */
}
