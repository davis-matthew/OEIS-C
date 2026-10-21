/*
 * A094730  Number of connected ordered 3-element multiantichains on a labeled
 *          n-set.
 * https://oeis.org/A094730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^n - 6*5^n + 3*4^n + 17*3^n - 30*2^n + 21, with a(0)=0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes base^exp where base is seq_t and exp is idx_t. */
static inline seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;

    seq_t p7 = pow_seq((seq_t)7, n);
    seq_t p5 = pow_seq((seq_t)5, n);
    seq_t p4 = pow_seq((seq_t)4, n);
    seq_t p3 = pow_seq((seq_t)3, n);
    seq_t p2 = pow_seq((seq_t)2, n);

    /* Sum positive terms: 7^n + 3*4^n + 17*3^n + 21 */
    seq_t pos = p7 + 3 * p4 + 17 * p3 + (seq_t)21;
    /* Sum negative terms: 6*5^n + 30*2^n */
    seq_t neg = 6 * p5 + 30 * p2;

    /* Final result: positive minus negative */
    return pos - neg;
}
