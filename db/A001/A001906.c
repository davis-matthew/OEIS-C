/*
 * A001906  F(2n) = bisection of Fibonacci sequence: a(n) = 3*a(n-1) - a(n-2).
 * https://oeis.org/A001906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, core
 * Formula:   a(n) = 3*a(n-1) - a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    seq_t prev2 = 0;
    seq_t prev1 = 1;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t curr = 3 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
