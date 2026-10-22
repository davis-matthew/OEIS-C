/*
 * A007018  a(n) = a(n-1)^2 + a(n-1), a(0)=1.
 * https://oeis.org/A007018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = a(n-1)^2 + a(n-1), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 0; i < n; i++) {
        result = result * (result + 1);
    }
    return result;
}
