/*
 * A000757  Number of cyclic permutations of [n] with no i -> i+1 (mod n).
 * https://oeis.org/A000757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n-2) * a(n-1) + (n-1) * a(n-2) - (-1)^n, a(0) = 1, a(1)
 *            = 0, a(2) = 0
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 0;
    if (n == 2) return 0;
    
    seq_t a_prev2 = 0;
    seq_t a_prev1 = 0;
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t a_i = (seq_t)(i - 2) * a_prev1 + (seq_t)(i - 1) * a_prev2;
        if (i & 1) {
            a_i = a_i + 1;
        } else {
            a_i = a_i - 1;
        }
        
        a_prev2 = a_prev1;
        a_prev1 = a_i;
    }
    
    return a_prev1;
}
