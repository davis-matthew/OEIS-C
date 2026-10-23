/*
 * A136293  Linear bound on the genera of Heegaard splittings of closed,
 *          orientable 3-manifolds that admit a generalized triangulation with
 *          n generalized tetrahedra.
 * https://oeis.org/A136293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 76*n + 26
 * Range:     u8 n <= 3, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * (seq_t)76 + (seq_t)26;
}
