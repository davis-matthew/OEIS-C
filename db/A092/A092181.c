/*
 * A092181  Figurate numbers based on the 24-cell (4-D polytope with Schlaefli
 *          symbol {3,4,3}).
 * https://oeis.org/A092181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2*(3*n^2 - 4*n + 2)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 194, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t t  = 3 * n2 - 4 * nn + 2;/* 3*n^2 - 4*n + 2 */
    return n2 * t;                /* n^2 * t == n^2*(3*n^2 - 4*n + 2) */
}
