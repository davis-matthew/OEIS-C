/*
 * A016776  a(n) = (3*n)^12.
 * https://oeis.org/A016776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n)^12
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 13, u128 n <= 541,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t base = (seq_t)3 * (seq_t)n;
    seq_t result = 1;
    for (int i = 0; i < 12; ++i)
        result *= base;
    return result;
}
