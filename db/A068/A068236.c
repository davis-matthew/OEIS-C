/*
 * A068236  First differences of (n+1)^5 - n^5.
 * https://oeis.org/A068236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*n^3 + 60*n^2 + 70*n + 30
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 597, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the second finite difference of the 5th powers: a(n) = (n+2)^5 - 2*(n+1)^5 + n^5 */
seq_t a(idx_t n)
{
    seq_t n_t   = (seq_t)n;
    seq_t n2    = n_t * n_t;       /* n^2 */
    seq_t n3    = n2 * n_t;        /* n^3 */
    return 20 * n3 + 60 * n2 + 70 * n_t + 30;
}
