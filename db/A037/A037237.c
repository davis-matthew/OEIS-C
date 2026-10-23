/*
 * A037237  Expansion of (3 + x^2) / (1 - x)^4.
 * https://oeis.org/A037237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(2*n^2 + 7*n + 9)/3
 * Range:     u8 n <= 5, u16 n <= 44, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n0 = (seq_t)n;
    seq_t n1 = n0 + 1;
    seq_t n2 = n0 * n0;
    seq_t t = 2 * n2 + 7 * n0 + 9;
    return n1 * t / 3;
}
