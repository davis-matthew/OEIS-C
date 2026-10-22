/*
 * A005605  a(n) = a(n-1) + (-1)^(n-1) * a(n-2)^2 for n >= 2 with a(0) = 0 and
 *          a(1) = 1.
 * https://oeis.org/A005605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1) + (-1)^(n-1) * a(n-2)^2, a(0) = 0, a(1) = 1
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 12, u64 n <= 14, u128 n <= 16,
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
        seq_t curr;
        if (i % 2 == 0) {
            // Even i: (-1)^(i-1) = -1
            curr = prev1 - prev2 * prev2;
        } else {
            // Odd i: (-1)^(i-1) = 1
            curr = prev1 + prev2 * prev2;
        }
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
