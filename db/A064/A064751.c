/*
 * A064751  a(n) = n*5^n - 1.
 * https://oeis.org/A064751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*5^n - 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t pow5 = (seq_t)1;
    seq_t five = (seq_t)5;
    idx_t i = 0;
    for (; i < n; ++i) {
        pow5 *= five;
    }
    seq_t res = pow5 * (seq_t)n;
    res -= (seq_t)1;
    return res;
}
