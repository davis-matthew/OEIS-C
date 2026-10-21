/*
 * A084367  a(n) = n*(2*n+1)^2.
 * https://oeis.org/A084367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(2*n+1)^2
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t twoN = N << 1;           /* 2*n */
    seq_t expr = twoN + ((seq_t)1);/* 2*n+1 */
    seq_t sq   = expr * expr;      /* (2*n+1)^2 */
    return N * sq;                 /* n*(2*n+1)^2 */
}
