/*
 * A016822  a(n) = (4n+1)^10.
 * https://oeis.org/A016822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4n+1)^10
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 20, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t base = (seq_t)4 * (seq_t)n + 1;
    seq_t result = 1;
    for (idx_t i = 0; i < 10; ++i) {
        result *= base;
    }
    return result;
}
