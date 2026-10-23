/*
 * A123900  a(n) = (n+3)!/(d(n)*d(n+1)*d(n+2)) where d(n) = cancellation
 *          factor in reducing Sum_{k=0...n} 1/k! to lowest terms.
 * https://oeis.org/A123900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+3)!/(d(n)*d(n+1)*d(n+2)) where d(m) = gcd(m!,P(m)) and
 *            the numerator P satisfies P(0)=1, P(m)=m*P(m-1)+1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

typedef unsigned __int128 big_t;       /* for intermediate big‑int arithmetic */

/* Euclidean gcd on big_t */
static big_t big_gcd(big_t a, big_t b)
{
    while (b)
    {
        big_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    const idx_t max_m = n + 2;        /* we need d(n), d(n+1), d(n+2) */
    big_t fac = 1;          /* running factorial m!  */
    big_t P   = 1;          /* current numerator of the sum   */
    big_t d_n_big = 1, d_n1_big = 1, d_n2_big = 1; /* d(n), d(n+1), d(n+2) */

    /* m = 0 case:
       fac = 1, P = 1, hence d(0) = gcd(1,1) = 1 -> already set */
    for (idx_t m = 1; m <= max_m; ++m)
    {
        fac = fac * (big_t)m;                /* fac = m!  */
        P   = (big_t)m * P + 1;              /* P(m)   */

        if (m == n)     d_n_big   = big_gcd(fac, P);
        if (m == n + 1) d_n1_big  = big_gcd(fac, P);
        if (m == n + 2) d_n2_big  = big_gcd(fac, P);
    }

    /* Now fac holds (n+2)!; multiply to get (n+3)! */
    fac = fac * (big_t)(n + 3);

    /* Denominator = d(n)*d(n+1)*d(n+2) */
    big_t denom_big = d_n_big * d_n1_big * d_n2_big;

    big_t result_big = fac / denom_big;

    return (seq_t)result_big;
}
