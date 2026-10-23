/*
 * A008532  Coordination sequence for 4-dimensional I-centered cubic
 *          orthogonal lattice.
 * https://oeis.org/A008532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^3 + 6*n, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t nn = (seq_t)n;
    seq_t n3 = nn * nn * nn;   /*  n^3  */
    return (seq_t)4 * n3 + (seq_t)6 * nn;
}
