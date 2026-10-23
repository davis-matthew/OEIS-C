/*
 * A035599  Number of points of L1 norm 5 in cubic lattice Z^n.
 * https://oeis.org/A035599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^4 + 20*n^2 + 6)*n/15
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 108, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;        /* n^2 */
    seq_t n4 = n2 * n2;        /* n^4 */
    seq_t num = ((seq_t)4 * n4 + (seq_t)20 * n2 + (seq_t)6) * nn; /* numerator */
    return num / (seq_t)15;   /* division is exact */
}
