/*
 * A005054  a(0) = 1; a(n) = 4*5^(n-1) for n >= 1.
 * https://oeis.org/A005054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*5^(n-1) for n >= 1, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t result = 4;
    for (idx_t i = 1; i < n; i++) {
        result *= 5;
    }
    return result;
}
