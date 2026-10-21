/*
 * A081108  8th binomial transform of (1,1,0,0,0,0,...).
 * https://oeis.org/A081108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n + 8)·8^(n-1), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow8(idx_t exp) {
    seq_t result = 1;      /* 8^0 = 1 */
    seq_t base   = 8;      /* base 8 */
    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    /* compute 8^(n-1) */
    seq_t pow_val = pow8(n - 1);
    /* (n + 8) * 8^(n-1) */
    return ((seq_t)n + (seq_t)8) * pow_val;
}
