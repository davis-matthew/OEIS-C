/*
 * A092880  Number of ordered 2-multiantichains on an n-set.
 * https://oeis.org/A092880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 2*3^n + 2*2^n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t p4 = pow_seq_t((seq_t)4, n);
    seq_t p3 = pow_seq_t((seq_t)3, n);
    seq_t p2 = pow_seq_t((seq_t)2, n);
    return p4 - (seq_t)2 * p3 + (seq_t)2 * p2;
}
