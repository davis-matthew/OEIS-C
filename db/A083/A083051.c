/*
 * A083051  First column of table A083050.
 * https://oeis.org/A083051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n * sqrt(2)/(sqrt(2) - 1)) + 1 * = 2*n +
 *            floor(n*sqrt(2)) + 1 * Uses the convergent 239/169 (error <
 *            10^(-12)) to compute floor(n*sqrt(2))
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t sqrt_term = (nseq * (seq_t)239) / (seq_t)169; /* floor(n*sqrt(2)) */
    seq_t val = 2 * nseq + sqrt_term + 1;
    return val;
}
