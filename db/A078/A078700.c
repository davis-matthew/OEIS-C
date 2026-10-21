/*
 * A078700  Number of symmetric ways to lace a shoe that has n pairs of
 *          eyelets such that each eyelet has at least one direct connection
 *          to the opposite side.
 * https://oeis.org/A078700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Fibonacci(n+1) (OEIS A078700)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* (n-1)! */
    seq_t fact = 1;
    for (idx_t i = 2; i < n; ++i) {
        fact *= (seq_t)i;
    }

    /* Fibonacci(n+1) using two consecutive values */
    seq_t fib_prev = 0;  /* F(0) */
    seq_t fib_curr = 1;  /* F(1) */
    for (idx_t i = 2; i <= n + 1; ++i) {
        seq_t tmp = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = tmp;
    }

    return fact * fib_curr;
}
