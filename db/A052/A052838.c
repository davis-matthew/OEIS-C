/*
 * A052838  Expansion of e.g.f.: (exp(x/(1-x)) - 1)^2.
 * https://oeis.org/A052838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 * sum_{j=0}^{n-1} C(n-1,j) * (2^j - 1) * (n! / (j+1)!)
 *            (n=0 gives a(0)=0). See OEIS A052838
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* factorial[n] */
    seq_t fact_n = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact_n = fact_n * (seq_t)i;

    seq_t temp = fact_n;          /* fact_n / fact_{j+1}  (j starts at 0) */
    seq_t pow2 = 1;               /* 2^j  (j starts at 0) */
    seq_t sum  = 0;
    seq_t bin  = 1;               /* C(n-1,0) */

    for (idx_t j = 0; j < n; ++j)
    {
        seq_t two_minus1 = pow2 - 1;          /* 2^j - 1 */
        seq_t term = bin * two_minus1;        /* C(n-1,j) * (2^j - 1) */
        term = term * temp;                   /* * (n!/(j+1)!) */
        sum += term;

        /* update for next j */
        pow2 = pow2 + pow2;                   /* 2^(j+1) */
        if (j + 1 < n)                       /* update binomial */
        {
            seq_t num = (seq_t)(n - 1 - j);
            seq_t den = (seq_t)(j + 1);
            bin = bin * num / den;           /* exact division */
        }
        if (j + 2 <= n)                      /* update temp ratio */
            temp = temp / (seq_t)(j + 2);     /* exact division */
    }

    return sum + sum;                         /* multiply by 2 */
}
