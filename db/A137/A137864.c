/*
 * A137864  a(n) = n^4 - 10n^3 + 35n^2 - 48n + 23.
 * https://oeis.org/A137864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^4 - 10n^3 + 35n^2 - 48n + 23
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 258, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n3 * N;
    seq_t res = n4 - (seq_t)10 * n3 + (seq_t)35 * n2 - (seq_t)48 * N + (seq_t)23;
    return res;
}
