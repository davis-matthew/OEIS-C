/*
 * A000459  Number of multiset permutations of {1, 1, 2, 2, ..., n, n} with no
 *          fixed points.
 * https://oeis.org/A000459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n*(2*n-1)*a(n-1) + 2*n*(n-1)*a(n-2) - (2*n-1), a(0) = 1,
 *            a(1) = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 0;
    
    seq_t a_prev2 = 1;  // a(0)
    seq_t a_prev1 = 0;  // a(1)
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t i_seq = (seq_t)i;
        seq_t a_curr = i_seq * (2 * i_seq - 1) * a_prev1 
                     + 2 * i_seq * (i_seq - 1) * a_prev2 
                     - (2 * i_seq - 1);
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    
    return a_prev1;
}
