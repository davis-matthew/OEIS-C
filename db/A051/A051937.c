/*
 * A051937  Truncated triangular pyramid numbers: a(n) = Sum_{k=4..n}
 *          (k*(k+1)/2 - 9).
 * https://oeis.org/A051937
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = (1/6)*(n-3)*(n^2+6*n-34)
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 2746, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;               /* n^2          */
    seq_t tmp = sq + 6 * nn - 34;     /* n^2 + 6n -34 */
    seq_t coef = (seq_t)(n - 3);       /* n-3          */
    seq_t prod = coef * tmp;           /* (n-3)*(n^2+6n-34) */
    return prod / 6;
}
