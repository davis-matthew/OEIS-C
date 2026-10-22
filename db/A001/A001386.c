/*
 * A001386  Coordination sequence for 4-dimensional I-centered tetragonal
 *          orthogonal lattice.
 * https://oeis.org/A001386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4n(4n^2 + 5) / 3 for n >= 1, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 725, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;
    return (4 * sn * (4 * n2 + 5)) / 3;
}
