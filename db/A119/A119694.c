/*
 * A119694  a(n) = Fibonacci(n) * Catalan(n).
 * https://oeis.org/A119694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(n) * Catalan(n)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute Fibonacci(n), with F(0)=0, F(1)=1 */
    seq_t fib_val;
    if (n == 0) {
        fib_val = 0;
    } else {
        seq_t f_prev = 0;
        seq_t f_curr = 1;
        for (idx_t i = 1; i <= n; ++i) {
            seq_t next = f_prev + f_curr;
            f_prev = f_curr;
            f_curr = next;
        }
        fib_val = f_prev;
    }

    /* Compute Catalan(n) multiplicatively: C(n) = binom(2n,n)/(n+1) */
    seq_t cat_val = 1;
    if (n > 0) {
        for (idx_t k = 1; k <= n; ++k) {
            cat_val = cat_val * (n + k) / k;
        }
        cat_val = cat_val / (n + 1);
    }

    return fib_val * cat_val;
}
