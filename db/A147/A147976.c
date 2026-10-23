/*
 * A147976  a(n) = 5^n-4^n-3^n-2^n-1.
 * https://oeis.org/A147976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5^(n-1) - 4^(n-1) - 3^(n-1) - 2^(n-1) - 1
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 14, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = base;
    while (exp) {
        if (exp & 1) result = result * b;
        b = b * b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n < 1 */
    idx_t e = n - 1;
    seq_t p5 = pow_seq_t(5, e);
    seq_t p4 = pow_seq_t(4, e);
    seq_t p3 = pow_seq_t(3, e);
    seq_t p2 = pow_seq_t(2, e);
    return p5 - p4 - p3 - p2 - 1;
}
