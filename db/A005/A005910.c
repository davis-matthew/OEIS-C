/*
 * A005910  Truncated octahedral numbers: 16*n^3 - 33*n^2 + 24*n - 6.
 * https://oeis.org/A005910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^3 - 33*n^2 + 24*n - 6
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 645, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    return 16 * n3 - 33 * n2 + 24 * nn - 6;
}
