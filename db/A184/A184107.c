/*
 * A184107  n-1+ceiling((n/6)^2); complement of A184106.
 * https://oeis.org/A184107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-1 + ceil((n/6)^2)
 * Range:     u8 n <= 15, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                   /* Convert index to seq_t */
    seq_t sq = nn * nn;                    /* n^2    */
    seq_t ceilSq36 = (sq + 35) / 36;       /* ceil(n^2/36) */
    return nn - 1 + ceilSq36;              /* a(n)   */
}
