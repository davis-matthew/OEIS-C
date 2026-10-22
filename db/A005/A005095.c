/*
 * A005095  a(n) = n! + n.
 * https://oeis.org/A005095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! + n
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    
    for (seq_t i = 2; i <= (seq_t)n; i++) {
        result *= i;
    }
    
    result += (seq_t)n;
    
    return result;
}
