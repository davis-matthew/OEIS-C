/*
 * A129695  Laguerre transform of the Jacobsthal numbers.
 * https://oeis.org/A129695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n!/3)*(LaguerreL(n,-2) - LaguerreL(n,1)) Recurrence:
 *            a(n) = (4*n-3)*a(n-1) - (6*n^2-16*n+9)*a(n-2) +
 *            (4*n-7)*(n-2)^2*a(n-3) - (n-2)^2*(n-3)^2*a(n-4), with a(0)=0,
 *            a(1)=1, a(2)=5, a(3)=30
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)30;

    seq_t a0 = (seq_t)0;   /* a(n-4) */
    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)5;   /* a(n-2) */
    seq_t a3 = (seq_t)30;  /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t Ni   = (seq_t)i;           /* current n */
        seq_t Ni_m2 = Ni - (seq_t)2;     /* n-2 */
        seq_t Ni_m3 = Ni - (seq_t)3;     /* n-3 */

        /* (4*n-3)*a(n-1) */
        seq_t t1 = ( (seq_t)4 * Ni - (seq_t)3 ) * a3;

        /* (6*n^2-16*n+9)*a(n-2) */
        seq_t n_sq = Ni * Ni;
        seq_t t2 = ( (seq_t)6 * n_sq - (seq_t)16 * Ni + (seq_t)9 ) * a2;

        /* (4*n-7)*(n-2)^2*a(n-3) */
        seq_t t3 = ( (seq_t)4 * Ni - (seq_t)7 )
                  * Ni_m2 * Ni_m2 * a1;

        /* (n-2)^2*(n-3)^2*a(n-4) */
        seq_t t4 = Ni_m2 * Ni_m2 * Ni_m3 * Ni_m3 * a0;

        seq_t an = t1 - t2 + t3 - t4;

        /* shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return a3;
}
