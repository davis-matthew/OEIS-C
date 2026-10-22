/*
 * A003215  Hex (or centered hexagonal) numbers: 3*n*(n+1)+1 (crystal ball
 *          sequence for hexagonal lattice).
 * https://oeis.org/A003215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3*n*(n+1) + 1
 * Range:     u8 n <= 8, u16 n <= 147, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    return 3 * n_seq * (n_seq + 1) + 1;
}
