/*
 * A027651  Poly-Bernoulli numbers B_n^(k) with k=-4.
 * https://oeis.org/A027651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*5^n - 36*4^n + 14*3^n - 2^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp) {
    /* Exponentiation by squaring – works for all supported seq_t types */
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t p5 = pow_seq(5, n);
    seq_t p4 = pow_seq(4, n);
    seq_t p3 = pow_seq(3, n);
    seq_t p2 = pow_seq(2, n);

    return 24 * p5 - 36 * p4 + 14 * p3 - p2;
}
