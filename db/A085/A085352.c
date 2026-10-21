/*
 * A085352  Expansion of (1-4*x)/((1-5*x)*(1-6*x)).
 * https://oeis.org/A085352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*6^n - 5^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == (idx_t)0) {
        return (seq_t)1;          /* 2*6^0 - 5^0 = 2*1 - 1 = 1 */
    }
    seq_t pow6 = pow_seq((seq_t)6, n);
    seq_t pow5 = pow_seq((seq_t)5, n);
    return (seq_t)2 * pow6 - pow5;
}
