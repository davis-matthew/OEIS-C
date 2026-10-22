/*
 * A004400  a(n) = 1 + Sum_{k=0..n} 2^k*k!.
 * https://oeis.org/A004400
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k=0..n} 2^k*k!, with recurrence a(n) = (2*n +
 *            1)*a(n-1) - (2*n)*a(n-2), a(-1) = 1, a(0) = 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == -1) return (seq_t)1;
    if (n == 0) return (seq_t)2;
    
    seq_t a_prev2 = (seq_t)1;
    seq_t a_prev1 = (seq_t)2;
    
    for (idx_t i = 1; i <= n; i++) {
        seq_t coeff1 = (seq_t)(2 * i + 1);
        seq_t coeff2 = (seq_t)(2 * i);
        seq_t a_curr = coeff1 * a_prev1 - coeff2 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    
    return a_prev1;
}
