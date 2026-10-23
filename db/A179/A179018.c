/*
 * A179018  Partial sums of ceiling(Fibonacci(n)/2).
 * https://oeis.org/A179018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + Fibonacci(n) + 1, a(0) = 0, a(1) = 1, a(2) = 2
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;

    /* buffer holding a(i-3) for each i (cycling indices 0..2) */
    seq_t a_buf[3] = {0, 1, 2};

    /* Fibonacci state: f_nm2 = Fib(i-2), f_nm1 = Fib(i-1) */
    seq_t f_nm2 = 1;   /* Fib(1) */
    seq_t f_nm1 = 1;   /* Fib(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t f_i = f_nm1 + f_nm2;                 /* Fib(i)   */
        seq_t a_i = a_buf[i % 3] + f_i + 1;        /* a(i) = a(i-3) + Fib(i) + 1 */
        a_buf[i % 3] = a_i;                        /* store a(i) for future use */
        f_nm2 = f_nm1;                             /* shift Fibonacci state */
        f_nm1 = f_i;
    }

    return a_buf[n % 3];
}
