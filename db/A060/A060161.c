/*
 * A060161  a(n) = 2^n - 1 + 2*Fibonacci(n-1).
 * https://oeis.org/A060161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n - 1 + 2*Fibonacci(n-1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t two_n = 1;
    for (idx_t i = 0; i < n; ++i) {
        two_n <<= 1;
    }
    two_n -= 1;   /* now two_n = 2^n - 1 */

    /* compute Fibonacci(n-1) */
    seq_t fib_nm1 = 0;
    if (n > 1) {
        seq_t a_f = 0, b_f = 1;
        for (idx_t i = 0; i < n - 1; ++i) {
            seq_t tmp = a_f + b_f;
            a_f = b_f;
            b_f = tmp;
        }
        fib_nm1 = a_f;   /* a_f holds F(n-1) */
    }

    /* a(n) = (2^n - 1) + 2*Fibonacci(n-1) */
    return two_n + (fib_nm1 << 1);
}
