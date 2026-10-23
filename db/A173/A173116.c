/*
 * A173116  a(n) = sinh(2*arcsinh(n))^2 = 4*n^2*(n^2 + 1).
 * https://oeis.org/A173116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^2*(n^2 + 1) -- OEIS A173116
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 181, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t before any arithmetic to avoid
       accidental promotion to the wrong integer type. */
    seq_t nn = (seq_t)n * (seq_t)n;        /* n^2   */
    seq_t term = 4 * nn * (nn + 1);        /* 4*n^2*(n^2+1) */
    return term;
}
