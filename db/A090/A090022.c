/*
 * A090022  Number of distinct lines through the origin in the n-dimensional
 *          lattice of side length 6.
 * https://oeis.org/A090022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7^n - 4^n - 3^n + 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1) result = result * b;
        b = b * b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* 7^0 - 4^0 - 3^0 + 1 = 0 */
    seq_t pow7 = pow_seq_t(7, n);
    seq_t pow4 = pow_seq_t(4, n);
    seq_t pow3 = pow_seq_t(3, n);
    seq_t res = pow7;
    res = res - pow4;
    res = res - pow3;
    res = res + 1;
    return res;
}
