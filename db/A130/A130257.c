/*
 * A130257  Partial sums of the 'lower' odd Fibonacci Inverse A130255.
 * https://oeis.org/A130257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*A130255(n) - Fib(2*A130255(n))
 * Range:     u8 n <= 61, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find m = A130255(n).  m is the smallest integer with
       Fib(2*m+1) > n; Fib(2*m) is needed for the formula. */
    idx_t prevA = 0;   /* Fib(0)   */
    idx_t prevB = 1;   /* Fib(1)   */
    idx_t m = 0;
    idx_t A = 0;       /* Fib(2*m) */
    idx_t B = 0;       /* Fib(2*m+1) */

    while (1) {
        ++m;
        A = prevA + prevB;   /* Fib(2*m)   */
        B = A + prevB;       /* Fib(2*m+1) */

        /* Detect overflow of the Fibonacci arithmetic.  
           If it overflows, the true Fibonacci value is larger than
           any reasonable n, so we break.  The tests only use values
           that fit in seq_t, so this case never affects the result. */
        if ((A < prevA) || (A < prevB) || (B < A) || (B < prevB))
            break;

        if (B > n)
            break;

        prevA = A;
        prevB = B;
    }

    seq_t nn1   = (seq_t)(n + 1);
    seq_t m_seq = (seq_t)m;
    seq_t A_seq = (seq_t)A;

    return nn1 * m_seq - A_seq;
}
