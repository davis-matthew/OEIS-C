/*
 * A167498  a(n) = 6+32*n^2+8*n*(7+8*n^2)/3.
 * https://oeis.org/A167498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 + 32*n^2 + 8*n*(7+8*n^2)/3
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 584, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;              /* Convert index to seq_t */
    seq_t n2 = tn * tn;               /* n^2 */
    seq_t term1 = (seq_t)6;           /* constant part */
    seq_t term2 = (seq_t)32 * n2;      /* 32*n^2 */
    /* 8*n*(7+8*n^2) is always divisible by 3 for all n */
    seq_t term3 = ( (seq_t)8 * tn * ((seq_t)7 + (seq_t)8 * n2) ) / (seq_t)3;
    return term1 + term2 + term3;
}
