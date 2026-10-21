/*
 * A099015  a(n) = Fib(n+1)*(2*Fib(n)^2 + Fib(n)*Fib(n-1) + Fib(n-1)^2).
 * https://oeis.org/A099015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + 6*a(n-2) - 3*a(n-3) - a(n-4) with a(0)=1,
 *            a(1)=2, a(2)=8, a(3)=33. (Equivalently a(n)=Fib(n+1)*(2*Fib(n)^2
 *            + Fib(n)*Fib(n-1)+Fib(n-1)^2).)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)33;

    /* Iteratively apply the linear recurrence */
    seq_t a0 = (seq_t)1;   /* a(n-4)   */
    seq_t a1 = (seq_t)2;   /* a(n-3)   */
    seq_t a2 = (seq_t)8;   /* a(n-2)   */
    seq_t a3 = (seq_t)33;  /* a(n-1)   */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t term = a3 * (seq_t)3;      /* 3*a(n-1) */
        term += a2 * (seq_t)6;           /* + 6*a(n-2) */
        term -= a1 * (seq_t)3;           /* - 3*a(n-3) */
        term -= a0;                      /* - a(n-4)   */

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = term;
    }
    return a3;
}
