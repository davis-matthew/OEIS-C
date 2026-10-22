/*
 * A001564  2nd differences of factorial numbers.
 * https://oeis.org/A001564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + n + 1)*n!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = (seq_t)n * (seq_t)n + (seq_t)n + 1;
    
    for (idx_t i = 2; i <= n; i++) {
        result = result * (seq_t)i;
    }
    
    return result;
}
