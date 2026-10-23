/*
 * A199751  a(n) = (3*11^n+1)/2.
 * https://oeis.org/A199751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*11^n+1)/2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t pow = 1;
    seq_t base = (seq_t)11;
    for (idx_t i = 0; i < n; ++i) {
        pow *= base;
    }
    seq_t result = pow * (seq_t)3 + (seq_t)1;
    result /= (seq_t)2;
    return result;
}
