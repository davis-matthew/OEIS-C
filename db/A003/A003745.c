/*
 * A003745  Number of spanning trees in (K_5 - e) X P_n.
 * https://oeis.org/A003745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 75*b(n)*c(n)^3, where b(n) = 5*b(n-1) - b(n-2), b(0) = 0,
 *            b(1) = 1 (A004254) and c(n) = 7*c(n-1) - c(n-2), c(0) = 0, c(1)
 *            = 1 (A004187)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t b0 = 0, b1 = 1, c0 = 0, c1 = 1;
    idx_t i;
    for (i = 1; i < n; i++) {
        seq_t bt = 5 * b1 - b0;
        seq_t ct = 7 * c1 - c0;
        b0 = b1; b1 = bt;
        c0 = c1; c1 = ct;
    }
    return (seq_t)75 * b1 * c1 * c1 * c1;
}
