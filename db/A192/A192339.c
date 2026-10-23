/*
 * A192339  Coefficient of x in the reduction of n-th polynomial at A157751 by
 *          x^2->x+2.
 * https://oeis.org/A192339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 10*a(n-3) - 4*a(n-4) for n ≥ 5
 *            (a(1)=0, a(2)=1, a(3)=3, a(4)=11)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)11;

    /* initialise a(1) … a(4) */
    seq_t a1 = (seq_t)0;   /* a(n-4) */
    seq_t a2 = (seq_t)1;   /* a(n-3) */
    seq_t a3 = (seq_t)3;   /* a(n-2) */
    seq_t a4 = (seq_t)11;  /* a(n-1) */
    seq_t an = (seq_t)0;

    for (idx_t i = 5; i <= n; ++i) {
        /* an = 4*a4 + a3 - 10*a2 - 4*a1; */
        an = (seq_t)4 * a4 + a3;
        an -= (seq_t)10 * a2;
        an -= (seq_t)4 * a1;

        /* shift window for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return an;
}
