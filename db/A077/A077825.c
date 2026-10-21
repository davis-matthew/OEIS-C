/*
 * A077825  Expansion of (1-x)^(-1)/(1-2*x-3*x^2-3*x^3).
 * https://oeis.org/A077825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 3*a(n-4) with initial values a(0)=1,
 *            a(1)=3, a(2)=10, a(3)=33
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)33;

    /* Use a sliding window of the last four results */
    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)3;   /* a(n-3) */
    seq_t a2 = (seq_t)10;  /* a(n-2) */
    seq_t a3 = (seq_t)33;  /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t s = ((seq_t)3 * a3) + a2;          /* 3*a(n-1) + a(n-2) */
        seq_t a4 = s - ((seq_t)3 * a0);          /* subtract 3*a(n-4) */

        /* Slide the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }

    return a3;    /* a(n) is stored in a3 after the loop */
}
