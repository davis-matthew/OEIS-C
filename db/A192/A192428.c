/*
 * A192428  Constant term of the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 10*a(n-2) - 6*a(n-3) - 9*a(n-4), initial
 *            values: a(0)=1, a(1)=1, a(2)=5, a(3)=11
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)11;

    /* Recurrence implementation using four rolling variables. */
    seq_t a0 = 1, a1 = 1, a2 = 5, a3 = 11, ax;
    idx_t i;
    for (i = 4; i <= n; ++i)
    {
        ax = 2*a3 + 10*a2 - 6*a1 - 9*a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ax;
    }
    return a3;
}
