/*
 * A142993  Crystal ball sequence for the lattice C_4.
 * https://oeis.org/A142993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n+1)^2*(4*n^2+4*n+3)/3
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 166, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn       = (seq_t)n;
    seq_t t2n1     = tn * 2 + 1;                  /* 2n + 1 */
    seq_t t2n1_sq  = t2n1 * t2n1;                 /* (2n+1)^2 */
    seq_t part     = (tn * tn) * 4 + tn * 4 + 3;  /* 4n^2 + 4n + 3 */
    seq_t numerator = t2n1_sq * part;             /* (2n+1)^2 * (4n^2+4n+3) */
    return numerator / 3;                         /* divide by 3 */
}
