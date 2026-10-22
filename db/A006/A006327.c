/*
 * A006327  a(n) = Fibonacci(n) - 3. Number of total preorders.
 * https://oeis.org/A006327
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = F(n) - 3, where F(n) is the n-th Fibonacci number
 *            (F(0)=0, F(1)=1)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t f_prev = 0;
    seq_t f_curr = 1;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }
    
    return f_curr - 3;
}
