/*
 * A005443  a(n) = n! * Fibonacci(n).
 * https://oeis.org/A005443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Fibonacci(n); computed via recurrence a(n) =
 *            n*a(n-1) + n*(n-1)*a(n-2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    seq_t prev2 = 0;  // a(0)
    seq_t prev1 = 1;  // a(1)
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t i_seq = (seq_t)i;
        seq_t curr = i_seq * prev1 + i_seq * (i_seq - 1) * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
