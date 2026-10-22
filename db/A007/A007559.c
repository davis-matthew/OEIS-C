/*
 * A007559  Triple factorial numbers (3*n-2)!!! with leading 1 added.
 * https://oeis.org/A007559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Product_{k=0..n-1} (3*k + 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t k = 0; k < n; k++) {
        result *= (3 * k + 1);
    }
    return result;
}
