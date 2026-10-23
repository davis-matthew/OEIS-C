/*
 * A017434  a(n) = (11*n + 3)^10.
 * https://oeis.org/A017434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 3)^10
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 7, u128 n <= 648,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t base = (seq_t)n * (seq_t)11 + (seq_t)3;
    seq_t result = 1;
    for (int i = 0; i < 10; ++i) result *= base;
    return result;
}
