/*
 * A002440  Squares written in base 7.
 * https://oeis.org/A002440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007093(A000290(n)), where A000290(n) = n^2 and A007093
 *            converts to base 7
 * Range:     u8 n <= 11, u16 n <= 128, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t square = n_seq * n_seq;
    
    // Convert square to base 7 and interpret as decimal
    seq_t result = 0;
    seq_t multiplier = 1;
    
    seq_t temp = square;
    while (temp > 0) {
        seq_t digit = temp % 7;
        result = result + digit * multiplier;
        multiplier = multiplier * 10;
        temp = temp / 7;
    }
    
    return result;
}
