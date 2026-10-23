/*
 * A192380  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) - 2*a(n-3) - a(n-4) for n >= 5 with
 *            initial values a(1)=0, a(2)=2, a(3)=4, a(4)=20
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* unused, for safety */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)20;

    /* keep the last four terms only */
    seq_t a1 = (seq_t)0; /* a(n-4) */
    seq_t a2 = (seq_t)2; /* a(n-3) */
    seq_t a3 = (seq_t)4; /* a(n-2) */
    seq_t a4 = (seq_t)20;/* a(n-1) */
    seq_t an = 0;

    for (idx_t k = 5; k <= n; ++k)
    {
        /* left  = 2*a4 + 6*a3   */
        seq_t left = (a4 << 1) + (a3 * (seq_t)6);
        /* right = 2*a2 + a1    */
        seq_t right = (a2 << 1) + a1;
        an = left - right;

        /* roll the window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return an;
}
