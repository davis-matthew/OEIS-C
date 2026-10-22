/*
 * A001924  Apply partial sum operator twice to Fibonacci numbers.
 * https://oeis.org/A001924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1) + a(n-2) + n, a(0) = 0, a(1) = 1
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 182,
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
        seq_t curr = prev1 + prev2 + (seq_t)i;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
