/*
 * A007805  a(n) = Fibonacci(6*n + 3)/2.
 * https://oeis.org/A007805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 18*a(n-1) - a(n-2), a(0) = 1, a(1) = 17
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 17;
    
    seq_t prev2 = 1;
    seq_t prev1 = 17;
    seq_t curr;
    
    for (idx_t i = 2; i <= n; i++) {
        curr = 18 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
