/*
 * A022385  Fibonacci sequence beginning 4, 22.
 * https://oeis.org/A022385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*Fibonacci(n+2) + 14*Fibonacci(n)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute Fibonacci(n) and Fibonacci(n+1) by iteration. */
    seq_t f_prev = 0;           /* F(0) */
    seq_t f_curr = 1;           /* F(1) */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t f_next = f_prev + f_curr; /* F(i+1) = F(i-1)+F(i) */
        f_prev = f_curr;               /* advance to F(i)   */
        f_curr = f_next;               /* advance to F(i+1) */
    }

    /* After the loop:
         f_prev = Fibonacci(n)
         f_curr = Fibonacci(n+1)
         Fibonacci(n+2) = f_prev + f_curr
    */
    seq_t f_np2 = f_prev + f_curr;          /* Fibonacci(n+2) */
    seq_t result = 4 * f_np2 + 14 * f_prev; /* a(n) */

    return result;
}
