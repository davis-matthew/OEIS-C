/*
 * A225007  Number of n X 5 0..1 arrays with rows unimodal and columns
 *          nondecreasing.
 * https://oeis.org/A225007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^5 + 35*n^4 + 115*n^3 + 175*n^2 + 121*n + 30) / 30
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 90, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert the index to seq_t for arithmetic */
    seq_t n0 = (seq_t)n;
    /* compute powers of n */
    seq_t n2 = n0 * n0;
    seq_t n3 = n2 * n0;
    seq_t n4 = n3 * n0;
    seq_t n5 = n4 * n0;
    /* assemble numerator */
    seq_t num = (seq_t)4 * n5
              + (seq_t)35 * n4
              + (seq_t)115 * n3
              + (seq_t)175 * n2
              + (seq_t)121 * n0
              + (seq_t)30;
    /* divide by 30 (exact division) */
    return num / (seq_t)30;
}
