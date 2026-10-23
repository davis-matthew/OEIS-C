/*
 * A123278  X-values of solutions to the equation 3(X-Y)^4 - 2*X*Y = 0 with X
 *          >= Y.
 * https://oeis.org/A123278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 108*a(n-1) - 982*a(n-2) + 108*a(n-3) - a(n-4) with
 *            initial values a(0)=0, a(1)=6, a(2)=500, a(3)=48114
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 6;
    if (n == 2) return 500;
    if (n == 3) return 48114;

    seq_t t0 = 0;          /* a(n-4) */
    seq_t t1 = 6;          /* a(n-3) */
    seq_t t2 = 500;        /* a(n-2) */
    seq_t t3 = 48114;      /* a(n-1) */
    seq_t tn;

    for (idx_t i = 4; i <= n; ++i)
    {
        tn = 108 * t3 - 982 * t2 + 108 * t1 - t0;
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = tn;
    }
    return t3;
}
