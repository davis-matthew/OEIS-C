/*
 * A193638  Number of permutations of the multiset
 *          {1,1,1,2,2,2,3,3,3,...,n,n,n} with no two consecutive terms equal.
 * https://oeis.org/A193638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*( (3*n-1)*(3*n^2-5*n+4)*a(n-1) +
 *            2*(n-1)*(6*n^2-9*n-1)*a(n-2) -4*n*(n-1)*(n-2)*a(n-3) )/(2*n-2)
 *            Initial values: a(0)=1, a(1)=0, a(2)=2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;

    seq_t a0 = (seq_t)1;  /* a(n-3) */
    seq_t a1 = (seq_t)0;  /* a(n-2) */
    seq_t a2 = (seq_t)2;  /* a(n-1) */
    seq_t an = a2;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t sn = (seq_t)i;

        /* term1 = (3*n-1)*(3*n^2-5*n+4)*a(n-1) */
        seq_t t1 = sn * 3 - 1;
        seq_t t2 = sn * sn * 3 - sn * 5 + 4;
        seq_t term1 = t1 * t2 * a2;

        /* term2 = 2*(n-1)*(6*n^2-9*n-1)*a(n-2) */
        seq_t t3 = sn - 1;
        seq_t t4 = sn * sn * 6 - sn * 9 - 1;
        seq_t term2 = 2 * t3 * t4 * a1;

        /* term3 = 4*n*(n-1)*(n-2)*a(n-3) */
        seq_t term3 = 4 * sn * (sn - 1) * (sn - 2) * a0;

        /* numerator = n * (term1 + term2 - term3) */
        seq_t num = sn * (term1 + term2 - term3);
        seq_t denom = (seq_t)(2 * i - 2); /* 2*(n-1) */

        an = num / denom;

        /* shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return an;
}
