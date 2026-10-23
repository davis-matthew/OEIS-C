/*
 * A017035  a(n) = (7*n + 4)^7.
 * https://oeis.org/A017035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 4)^7
 * Range:     u8 none, u16 n <= 0, u32 n <= 2, u64 n <= 80, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t base = (seq_t)7 * (seq_t)n + (seq_t)4;
    seq_t result = 1;
    for (idx_t i = 0; i < 7; ++i) {
        result *= base;
    }
    return result;
}
