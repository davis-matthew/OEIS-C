/*
 * A008355  Coordination sequence for D_5 lattice.
 * https://oeis.org/A008355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(9*n^2+1)*(n^2+1) (a(0) = 1 by definition)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 124, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }
    seq_t n_seq = (seq_t)n;
    seq_t n_sq   = n_seq * n_seq;          /* n^2 */
    seq_t part1  = 9 * n_sq + 1;           /* 9*n^2 + 1 */
    seq_t part2  = n_sq + 1;               /* n^2 + 1 */
    return 2 * part1 * part2;
}
