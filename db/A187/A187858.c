/*
 * A187858  Number of 3-step one space for components leftwards or up, two
 *          space for components rightwards or down asymmetric quasi-queen's
 *          tours (antidiagonal moves become knight moves) on an n X n board
 *          summed over all starting positions.
 * https://oeis.org/A187858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 33, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A187858: Number of 3-step one-space for components leftwards or up, 
 * two-space for components rightwards or down asymmetric quasi-queen's tours 
 * (antidiagonal moves become knight moves) on an n X n board summed over 
 * all starting positions.
 *
 * Empirical formula (valid for n>3):
 *   a(n) = 64*n^2 - 252*n + 238
 *
 * Initial values:
 *   a(1) = 0
 *   a(2) = 2
 *   a(3) = 81
 */
seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 2;
    if (n == 3) return 81;
    seq_t nn = (seq_t)n;
    return 64 * nn * nn - 252 * nn + 238;
}
