/*
 * A004215  Numbers that are the sum of 4 but no fewer nonzero squares.
 * https://oeis.org/A004215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   Numbers of the form 4^i*(8*j+7), i >= 0, j >= 0
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t k = 0;
    
    while (count < (seq_t)n) {
        k++;
        seq_t temp = k;
        
        // Remove all factors of 4
        while (temp % 4 == 0) {
            temp /= 4;
        }
        
        // Check if temp ≡ 7 (mod 8)
        if (temp % 8 == 7) {
            count++;
        }
    }
    
    return k;
}
