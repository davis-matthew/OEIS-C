/*
 * A007689  a(n) = 2^n + 3^n.
 * https://oeis.org/A007689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2^n + 3^n
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    return pow_seq(2, n) + pow_seq(3, n);
}
