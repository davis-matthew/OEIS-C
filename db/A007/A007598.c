/*
 * A007598  Squared Fibonacci numbers: a(n) = F(n)^2 where F = A000045.
 * https://oeis.org/A007598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = F(n)^2, F = A000045
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    seq_t f_prev = 0;
    seq_t f_curr = 1;
    for (idx_t i = 2; i <= n; i++) {
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }
    
    return f_curr * f_curr;
}
