/*
 * A020541  a(n) = 4th Chebyshev polynomial (second kind) evaluated at 2^n.
 * https://oeis.org/A020541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16^(n+1) - 3*4^(n+1) + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 16^(n+1) and 4^(n+1) by simple iteration */
    seq_t p16 = 1;
    seq_t p4  = 1;
    idx_t m = n + 1;          /* loop count */
    for (idx_t i = 0; i < m; ++i) {
        p16 = p16 * (seq_t)16;
        p4  = p4  * (seq_t)4;
    }
    /* combine to get the final result */
    seq_t res = p16 - 3 * p4 + 1;
    return res;
}
