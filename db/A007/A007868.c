/*
 * A007868  Number of inverse pairs of elements in symmetric group S_n, or
 *          pairs of total orders on n nodes (average of A000085 and A000142).
 * https://oeis.org/A007868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (A000085(n) + n!) / 2, where A000085(n) is the number of
 *            involutions in S_n, satisfying A000085(n) = A000085(n-1) +
 *            (n-1)*A000085(n-2) with A000085(0) = A000085(1) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return 1;
    
    seq_t inv_prev2 = 1;  // inv(0)
    seq_t inv_prev1 = 1;  // inv(1)
    seq_t inv_curr;
    seq_t fact = 1;
    
    for (idx_t i = 2; i <= n; i++) {
        inv_curr = inv_prev1 + (seq_t)(i - 1) * inv_prev2;
        fact *= (seq_t)i;
        inv_prev2 = inv_prev1;
        inv_prev1 = inv_curr;
    }
    
    return (inv_curr + fact) / 2;
}
