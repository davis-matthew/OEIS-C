/*
 * A058038  a(n) = Fibonacci(2*n)*Fibonacci(2*n+2).
 * https://oeis.org/A058038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n)*Fibonacci(2*n+2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence implementation: a(n) = 8*a(n-1) - 8*a(n-2) + a(n-3), a(0) = 0, a(1) = 3, a(2) = 24 */

seq_t a(idx_t n)
{
    /* Handle the initial terms directly */
    if (n == 0) return 0;
    if (n == 1) return 3;
    if (n == 2) return 24;

    /* We only need the last three values of the sequence at any time */
    seq_t a0 = 0;   /* a(n-3) */
    seq_t a1 = 3;   /* a(n-2) */
    seq_t a2 = 24;  /* a(n-1) */
    seq_t a3;       /* a(n) for current n */

    /* Compute iteratively from 3 up to the requested n */
    for (idx_t i = 3; i <= n; ++i)
    {
        a3 = (a2 * 8) - (a1 * 8) + a0;   /* a(i) = 8*a(i-1) - 8*a(i-2) + a(i-3) */
        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }
    return a2;
}
