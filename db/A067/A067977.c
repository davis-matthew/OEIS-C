/*
 * A067977  Convolution of Fibonacci F(n+1), n>=0, with F(n+9), n>=0.
 * https://oeis.org/A067977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = 2*a(n-1) + a(n-2) -
 *            2*a(n-3) - a(n-4) with initial values a(0) = 34, a(1) = 89, a(2)
 *            = 212, a(3) = 445. This follows from the generating function
 *            (34+21*x)/(1-x-x^2)^2
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 0) return 34;
    if (n == 1) return 89;
    if (n == 2) return 212;
    if (n == 3) return 445;

    /* Iteratively compute from a(0) up to a(n). */
    seq_t a0 = 34, a1 = 89, a2 = 212, a3 = 445;  /* a(n-4), a(n-3), a(n-2), a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t term = a3 + a3;          /* 2*a(n-1)          */
        term = term + a2;              /* 2*a(n-1) + a(n-2) */
        term = term - (a1 + a1);       /* -2*a(n-3)         */
        term = term - a0;              /* -a(n-4)           */

        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = term;
    }
    return a3;  /* a(n) */
}
