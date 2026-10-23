/*
 * A106804  Expansion of g.f.: x*(2 - 9*x - 4*x^2)/((1 - 5*x + x^2)*(1 - 5*x -
 *          x^2)).
 * https://oeis.org/A106804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=2, a(2)=11, a(3)=56,
 *            a(n)=10*a(n-1)-25*a(n-2)+a(n-4) for n>=4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)56;

    seq_t a0 = (seq_t)0;   /* a(n-4) */
    seq_t a1 = (seq_t)2;   /* a(n-3) */
    seq_t a2 = (seq_t)11;  /* a(n-2) */
    seq_t a3 = (seq_t)56;  /* a(n-1) */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i)
    {
        /* Compute a(i) = a0 + 10*a3 - 25*a2 */
        cur = a0 + a3 * 10 - a2 * 25;

        /* Shift window: a(n-3) becomes a(n-2), etc. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = cur;
    }
    return cur;
}
