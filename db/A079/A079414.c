/*
 * A079414  a(n) = 4*n^4 - 3*n^2.
 * https://oeis.org/A079414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^4 - 3*n^2
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 181, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid accidental truncation. */
    seq_t nn = (seq_t)n;
    /* Compute n^2 first. */
    seq_t sq = nn * nn;
    /* Compute 4*n^4 and 3*n^2, then subtract. */
    seq_t term1 = 4 * sq * sq;   /* 4*n^4 */
    seq_t term2 = 3 * sq;        /* 3*n^2 */
    return term1 - term2;
}
