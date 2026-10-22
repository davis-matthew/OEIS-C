/*
 * A003470  a(n) = n*a(n-1) - a(n-2) + 1 + (-1)^n.
 * https://oeis.org/A003470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*a(n-1) - a(n-2) + 1 + (-1)^n, a(0) = 1, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 1;
    
    seq_t prev2 = 1;
    seq_t prev1 = 1;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t curr;
        if (i & 1) {
            curr = (seq_t)i * prev1 - prev2;
        } else {
            curr = (seq_t)i * prev1 - prev2 + 2;
        }
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
