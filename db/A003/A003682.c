/*
 * A003682  Number of (undirected) Hamiltonian paths in the n-ladder graph K_2
 *          X P_n.
 * https://oeis.org/A003682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = n^2 - n + 2 for n >= 2
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)1;
    }
    seq_t sn = (seq_t)n;
    return sn * sn - sn + 2;
}
