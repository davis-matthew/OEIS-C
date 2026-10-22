/*
 * A000680  a(n) = (2n)!/2^n.
 * https://oeis.org/A000680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n)!/2^n = n! * Product_{i=0..n-1} (2i+1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 0; i < n; i++) {
        result *= (seq_t)(2 * i + 1);
        result *= (seq_t)(i + 1);
    }
    return result;
}
