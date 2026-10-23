/*
 * A048395  Sum of consecutive nonsquares.
 * https://oeis.org/A048395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2*n^3 + 2*n^2 + n
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1289, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* compute 2*n^3 */
    seq_t term = nn * nn;          /* n^2 */
    term = term * nn;              /* n^3 */
    term = term * (seq_t)2;        /* 2*n^3 */
    /* compute 2*n^2 */
    seq_t term2 = nn * nn;         /* n^2 */
    term2 = term2 * (seq_t)2;      /* 2*n^2 */

    /* sum all parts */
    return term + term2 + nn;
}
