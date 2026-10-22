/*
 * A000538  Sum of fourth powers: 0^4 + 1^4 + ... + n^4.
 * https://oeis.org/A000538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(1+n)*(1+2*n)*(-1+3*n+3*n^2)/30
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 115, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t np1 = nn + 1;
    seq_t n2p1 = 2*nn + 1;
    seq_t poly = 3*nn*nn + 3*nn - 1;
    return nn * np1 * n2p1 * poly / 30;
}
