/*
 * A005701  Number of exterior points formed by extending diagonals of n-gon
 *          in general position.
 * https://oeis.org/A005701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(n+3)*(n+6)/12
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 317, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_cast = (seq_t)n;
    seq_t part1 = ((n_cast + 1) * (n_cast + 2)) / 2;
    seq_t part2 = (n_cast + 3) * (n_cast + 6);
    return part1 * part2 / 6;
}
