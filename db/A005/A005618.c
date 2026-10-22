/*
 * A005618  a(n) = 6*a(n-1) - 8, with a(0) = 4.
 * https://oeis.org/A005618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*6^n + 8)/5
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t power_6 = pow_seq((seq_t)6, n);
    seq_t numerator = (seq_t)12 * power_6 + (seq_t)8;
    return numerator / (seq_t)5;
}
