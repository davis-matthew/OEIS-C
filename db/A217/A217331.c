/*
 * A217331  Number of inequivalent ways to color a 3 X 3 checkerboard using at
 *          most n colors allowing rotations and reflections.
 * https://oeis.org/A217331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^9 + n^5 + 2*n^3 + 4*n^6)/8
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t z = (seq_t)n;
    seq_t n2 = z * z;
    seq_t n3 = n2 * z;
    seq_t n5 = n3 * n2;
    seq_t n6 = n5 * z;
    seq_t n9 = n6 * n3;
    seq_t numerator = n9 + n5 + (2 * n3) + (4 * n6);
    return numerator / ((seq_t)8);
}
