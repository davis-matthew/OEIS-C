/*
 * A183654  Number of (n+1) X 2 0..5 arrays with every 2 X 2 subblock summing
 *          to 10.
 * https://oeis.org/A183654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*6^n + 10*5^n + 8*4^n + 6*3^n + 4*2^n + 2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t p2 = pow_seq(2, n);
    seq_t p3 = pow_seq(3, n);
    seq_t p4 = pow_seq(4, n);
    seq_t p5 = pow_seq(5, n);
    seq_t p6 = pow_seq(6, n);
    return p6 * 6 + p5 * 10 + p4 * 8 + p3 * 6 + p2 * 4 + 2;
}
