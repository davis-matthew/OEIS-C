/*
 * A224803  Number of (n+2)X3 0..3 matrices with each 3X3 subblock having the
 *          same population.
 * https://oeis.org/A224803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(floor((n+4)/3)) * T(floor((n+3)/3)) *
 *            T(floor((n+2)/3)), where T(k) = 4 + 12*3^k + 4*6^k
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 20, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

static inline seq_t T(idx_t k) {
    seq_t p3 = pow_seq((seq_t)3, k);
    seq_t p6 = pow_seq((seq_t)6, k);
    return (seq_t)4 + (seq_t)12 * p3 + (seq_t)4 * p6;
}

seq_t a(idx_t n) {
    idx_t k0 = (n + (idx_t)4) / (idx_t)3;
    idx_t k1 = (n + (idx_t)3) / (idx_t)3;
    idx_t k2 = (n + (idx_t)2) / (idx_t)3;
    seq_t t0 = T(k0);
    seq_t t1 = T(k1);
    seq_t t2 = T(k2);
    seq_t res = t0;
    res *= t1;
    res *= t2;
    return res;
}
