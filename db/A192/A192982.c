/*
 * A192982  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(n+3) + Fibonacci(n+2) - (n^2 + 2*n + 5)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute Fibonacci numbers up to F(n+3) iteratively. */
    seq_t f_prev = 0;     /* F(0) */
    seq_t f_curr = 1;     /* F(1) */
    idx_t target = n + 3; /* we need F(n+2) and F(n+3) */

    for (idx_t i = 2; i <= target; ++i) {
        seq_t f_next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = f_next;
    }

    /* After the loop:
       f_prev = F(n+2), f_curr = F(n+3)   */
    seq_t f_n2 = f_prev;
    seq_t f_n3 = f_curr;

    /* Lucas(n+3) = 2*F(n+2) + F(n+3) */
    seq_t lucas = 2 * f_n2 + f_n3;

    /* a(n) = Lucas(n+3) + F(n+2) - (n^2 + 2*n + 5) */
    seq_t n_seq = (seq_t)n;
    seq_t poly  = n_seq * n_seq + 2 * n_seq + 5;
    seq_t result = lucas + f_n2 - poly;

    return result;
}
