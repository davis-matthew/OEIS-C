/*
 * A002720  Number of partial permutations of an n-set; number of n X n binary
 *          matrices with at most one 1 in each row and column.
 * https://oeis.org/A002720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = 2*n*a(n-1) - (n-1)^2*a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;
    
    seq_t prev2 = 1;
    seq_t prev1 = 2;
    seq_t curr;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t si = (seq_t)i;
        seq_t si_minus_1 = si - 1;
        curr = 2 * si * prev1 - si_minus_1 * si_minus_1 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
