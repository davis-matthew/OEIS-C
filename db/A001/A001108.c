/*
 * A001108  a(n)-th triangular number is a square: a(n+1) = 6*a(n) - a(n-1) +
 *          2, with a(0) = 0, a(1) = 1.
 * https://oeis.org/A001108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 6*a(n-1) - a(n-2) + 2, a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    seq_t a_prev2 = 0;
    seq_t a_prev1 = 1;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t a_curr = 6 * a_prev1 - a_prev2 + 2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    
    return a_prev1;
}
