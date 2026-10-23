/*
 * A192973  Constant term of the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4) Initial values:
 *            a(1) = 1, a(2) = 3, a(3) = 10, a(4) = 23
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)10;
    if (n == 4) return (seq_t)23;

    /* Sliding window of the last four values */
    seq_t a1 = (seq_t)1;   /* a(n-4) */
    seq_t a2 = (seq_t)3;   /* a(n-3) */
    seq_t a3 = (seq_t)10;  /* a(n-2) */
    seq_t a4 = (seq_t)23;  /* a(n-1) */
    seq_t an;              /* a(n) */

    for (idx_t i = 5; i <= n; ++i) {
        /* an = 3*a4 - 2*a3 - a2 + a1 */
        seq_t temp = (seq_t)3 * a4;
        temp = temp - (seq_t)2 * a3;
        temp = temp - a2;
        temp = temp + a1;
        an = temp;

        /* Shift the window for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }

    return a4;
}
