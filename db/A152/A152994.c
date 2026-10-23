/*
 * A152994  Nine times hexagonal numbers: a(n) = 9*n*(2*n-1).
 * https://oeis.org/A152994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n*(2*n-1)
 * Range:     u8 n <= 4, u16 n <= 60, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;         /* convert index to seq_t */
    seq_t temp = (seq_t)2 * nn - (seq_t)1;   /* 2*n - 1 */
    return nn * ((seq_t)9 * temp);
}
