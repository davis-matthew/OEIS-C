/*
 * A121955  Expansion of x^2*(9 + 8*x - 8*x^2)/((1+x-x^2)*(1-2*x-4*x^2)).
 * https://oeis.org/A121955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 7*a(n-2) + 2*a(n-3) - 4*a(n-4), with the initial
 *            values a(1)=0, a(2)=9, a(3)=17, a(4)=72
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)17;
    if (n == 4) return (seq_t)72;

    seq_t a1 = (seq_t)0;   /* a(n-4) */
    seq_t a2 = (seq_t)9;   /* a(n-3) */
    seq_t a3 = (seq_t)17;  /* a(n-2) */
    seq_t a4 = (seq_t)72;  /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t a5 = a4
                 + ((seq_t)7 * a3)
                 + ((seq_t)2 * a2)
                 - ((seq_t)4 * a1);

        a1 = a2;  /* shift the window for next iteration */
        a2 = a3;
        a3 = a4;
        a4 = a5;
    }
    return a4;
}
