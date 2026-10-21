/*
 * A052995  Expansion of 2*x*(1 - x)/(1 - 3*x + x^2).
 * https://oeis.org/A052995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A052995: a(n) = 2*Fibonacci(2n-1)  (for n>0)
   Recurrence: a(0)=0, a(1)=2, a(2)=4,
                a(n) = 3*a(n-1) - a(n-2)  for n≥3 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    /* a(n-2) and a(n-1) for the recurrence */
    seq_t a_nm2 = (seq_t)2; /* a(1) */
    seq_t a_nm1 = (seq_t)4; /* a(2) */
    seq_t a_n   = a_nm1;    /* placeholder */

    for (idx_t i = 3; i <= n; ++i)
    {
        a_n = 3 * a_nm1;
        a_n -= a_nm2;     /* a(n) = 3*a(n-1) - a(n-2) */

        a_nm2 = a_nm1;    /* shift the window */
        a_nm1 = a_n;
    }

    return a_n;
}
