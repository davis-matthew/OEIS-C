/*
 * A082033  a(n) = (3*n + 1)*n!.
 * https://oeis.org/A082033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (3*n + 1)*n!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fact *= i;
    }
    seq_t mul = (seq_t)3 * n + 1;
    return fact * mul;
}
