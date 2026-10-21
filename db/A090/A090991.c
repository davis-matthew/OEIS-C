/*
 * A090991  Number of meaningful differential operations of the n-th order on
 *          the space R^6.
 * https://oeis.org/A090991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*Fibonacci(n+3) where Fibonacci(0)=0, Fibonacci(1)=1.
 *            This is equivalent to a(1)=6, a(2)=10, and for n>2:
 *            a(n)=a(n-1)+a(n-2)
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Undefined indices return 0. */
    if (n < 1)
        return (seq_t)0;

    seq_t f0 = 0;             /* Fibonacci(0) */
    seq_t f1 = 1;             /* Fibonacci(1) */
    idx_t target = n + 3;     /* Need Fibonacci(n+3) */

    for (idx_t i = 2; i <= target; ++i)
    {
        seq_t tmp = f0 + f1;  /* Fibonacci(i) = f0 + f1 */
        f0 = f1;
        f1 = tmp;
    }

    return f1 * 2;            /* a(n) = 2 * Fibonacci(n+3) */
}
