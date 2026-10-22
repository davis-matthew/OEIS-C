/*
 * A007583  a(n) = (2^(2*n + 1) + 1)/3.
 * https://oeis.org/A007583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 1, a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t v = 1;
    idx_t i;
    for (i = 0; i < n; i++) {
        v = v * 4 - 1;
    }
    return v;
}
