/*
 * A087719  Least number m such that the number of numbers k <= m with k >
 *          lpf(k)^n exceeds the number of numbers with k <= lpf(k)^n, where
 *          lpf = A020639.
 * https://oeis.org/A087719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + 3*2^n + 6
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset 1 – n>=1 in the tests */
    seq_t pow3 = pow_seq_t(3, n);
    seq_t pow2 = pow_seq_t(2, n);
    return pow3 + 3 * pow2 + 6;
}
