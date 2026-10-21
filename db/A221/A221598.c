/*
 * A221598  Number of 0..n arrays of length 6 with each element differing from
 *          at least one neighbor by 1 or less.
 * https://oeis.org/A221598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27*n^3 + 108*n^2 - 252*n + 267 for n > 3 a(1) = 64, a(2)
 *            = 393, a(3) = 1210
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the tiny cases explicitly */
    if (n == 1) return (seq_t)64;
    if (n == 2) return (seq_t)393;
    if (n == 3) return (seq_t)1210;

    /* For n >= 4 use the closed‑form polynomial */
    seq_t nn = (seq_t)n;
    seq_t term1 = (seq_t)27 * nn * nn * nn;  /* 27*n^3 */
    seq_t term2 = (seq_t)108 * nn * nn;       /* 108*n^2 */
    seq_t term3 = (seq_t)252 * nn;            /* 252*n */
    return term1 + term2 - term3 + (seq_t)267;
}
