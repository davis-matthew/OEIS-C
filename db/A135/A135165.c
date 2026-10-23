/*
 * A135165  a(n) = 7^n + 5^n - 3^n - 2^n.
 * https://oeis.org/A135165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n + 5^n - 3^n - 2^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation uses binary exponentiation to avoid a linear loop. */
static inline seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t p7 = pow_seq((seq_t)7, n);
    seq_t p5 = pow_seq((seq_t)5, n);
    seq_t p3 = pow_seq((seq_t)3, n);
    seq_t p2 = pow_seq((seq_t)2, n);
    return p7 + p5 - p3 - p2;
}
