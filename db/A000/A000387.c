/*
 * A000387  Rencontres numbers: number of permutations of [n] with exactly two
 *          fixed points.
 * https://oeis.org/A000387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*a(n-1) + (-1)^n*n*(n-1)/2, a(0) = 0
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    
    seq_t result = 0;
    
    for (idx_t i = 1; i <= n; i++) {
        seq_t si = (seq_t)i;
        seq_t new_result = si * result;
        
        // Add (-1)^i * i * (i-1) / 2
        seq_t term = si * (si - 1) / 2;
        if (i % 2 == 0) {
            new_result = new_result + term;
        } else {
            new_result = new_result - term;
        }
        
        result = new_result;
    }
    
    return result;
}
