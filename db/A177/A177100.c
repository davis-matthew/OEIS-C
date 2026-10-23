/*
 * A177100  Partial sums of round(n^2/9).
 * https://oeis.org/A177100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+3)*(2*n^2 - 3*n + 6) / 54)
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1478, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the OEIS sequence A177100: partial sums of round(n^2/9). */
seq_t a(idx_t n)
{
    seq_t nn = n;
    /* Compute 2*n^2 - 3*n + 6 */
    seq_t two_n_sq = 2 * nn * nn;
    seq_t minus_three_n = 3 * nn;
    seq_t const_six = 6;
    seq_t inner = two_n_sq - minus_three_n + const_six;
    /* Multiply by (n + 3) */
    seq_t multiplier = nn + 3;
    seq_t num = multiplier * inner;
    /* Divide by 54 */
    return num / 54;
}
