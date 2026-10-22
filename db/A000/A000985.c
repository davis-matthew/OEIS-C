/*
 * A000985  Number of n X n symmetric matrices with nonnegative entries and
 *          all row sums 2.
 * https://oeis.org/A000985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   2*a(n) = 2*(2*n-1)*a(n-1) - 2*(n-2)*(n-1)*a(n-2) -
 *            2*(n-2)*(n-1)*a(n-3) + (n-3)*(n-2)*(n-1)*a(n-4)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 3;
    if (n == 3) return 11;
    
    seq_t a0 = 1, a1 = 1, a2 = 3, a3 = 11;
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t n_s = (seq_t)i;
        seq_t n_2_s = (seq_t)(i - 2);
        seq_t n_1_s = (seq_t)(i - 1);
        seq_t n_3_s = (seq_t)(i - 3);
        
        seq_t result;
        result = (2 * n_s - 1) * a3;
        result -= n_2_s * n_1_s * a2;
        result -= n_2_s * n_1_s * a1;
        result += n_3_s * n_2_s * n_1_s / 2 * a0;
        
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = result;
    }
    
    return a3;
}
