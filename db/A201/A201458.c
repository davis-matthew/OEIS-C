/*
 * A201458  Expansion of 1/((1-2*x)*(1-3*x+3*x^2)*(1-4*x+6*x^2-4*x^3)).
 * https://oeis.org/A201458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            9*a(n-1)-35*a(n-2)+76*a(n-3)-98*a(n-4)+72*a(n-5)-24*a(n-6);
 *            initial values: a(0)=1, a(-1)=a(-2)=a(-3)=a(-4)=a(-5)=0
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 24, u64 n <= 55, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* a(i-1), a(i-2), a(i-3), a(i-4), a(i-5), a(i-6) */
    seq_t r0 = 1;   /* a(0) */
    seq_t r1 = 0;   /* a(-1) */
    seq_t r2 = 0;   /* a(-2) */
    seq_t r3 = 0;   /* a(-3) */
    seq_t r4 = 0;   /* a(-4) */
    seq_t r5 = 0;   /* a(-5) */

    for (idx_t i = 1; i <= n; ++i) {
        /* compute 9*a(i-1) + 76*a(i-3) + 72*a(i-5) - (35*a(i-2) + 98*a(i-4) + 24*a(i-6)) */
        seq_t pos = 9 * r0 + 76 * r2 + 72 * r4;
        seq_t neg = 35 * r1 + 98 * r3 + 24 * r5;
        seq_t an  = pos - neg;        /* pos >= neg for all n >= 0, so unsigned subtraction is safe */

        /* shift window for next iteration */
        r5 = r4;
        r4 = r3;
        r3 = r2;
        r2 = r1;
        r1 = r0;
        r0 = an;
    }
    return r0;
}
