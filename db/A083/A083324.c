/*
 * A083324  a(n) = 4^n - 3^n + 2^n.
 * https://oeis.org/A083324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 3^n + 2^n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t power_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t p4 = power_seq((seq_t)4, n);
    seq_t p3 = power_seq((seq_t)3, n);
    seq_t p2 = power_seq((seq_t)2, n);
    seq_t t = p4 - p3;
    t += p2;
    return t;
}
