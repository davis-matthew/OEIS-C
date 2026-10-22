/*
 * A003751  Number of spanning trees in K_5 x P_n.
 * https://oeis.org/A003751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 125*A004187(n)^4, where A004187(m) = 7*A004187(m-1) -
 *            A004187(m-2), A004187(0) = 0, A004187(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c0 = 0, c1 = 1;
    idx_t i;
    for (i = 1; i < n; i++) {
        seq_t c2 = (seq_t)7 * c1 - c0;
        c0 = c1;
        c1 = c2;
    }
    seq_t c2sq = c1 * c1;
    return (seq_t)125 * (c2sq * c2sq);
}
