/*
 * A173965  Averages of four consecutive cubes.
 * https://oeis.org/A173965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1)*(n^2-n+4)/2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                   /* cast n to seq_t */
    seq_t term1 = (seq_t)2 * nn - 1;       /* 2*n-1  (always odd) */
    seq_t term2 = nn * nn - nn + 4;        /* n^2-n+4  (always even) */
    seq_t term2_half = term2 / 2;           /* divide by 2 safely */
    return term1 * term2_half;             /* product gives the result */
}
