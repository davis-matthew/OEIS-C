/*
 * A145609  Numerator of the polynomial A_l(x) = sum_{d=1..l-1} x^(l-d)/d for
 *          index l=2n+1 evaluated at x=1.
 * https://oeis.org/A145609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of H_{2n} = sum_{k=1}^{2n} 1/k in lowest terms
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   gcd, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* length of harmonic number: H_{2n} */
    idx_t m = n + n;          /* 2 * n */

    seq_t num = 0;            /* current numerator (0/1) */
    seq_t den = 1;            /* current denominator (0/1) */

    for (idx_t k = 1; k <= m; ++k)
    {
        /* add 1/k to the current fraction num/den */
        seq_t g  = gcd(den, (seq_t)k);          /* gcd(den, k) */
        seq_t t1 = k / g;                       /* factor of k coprime with den */
        seq_t t2 = den / g;                     /* factor of den coprime with k */

        /* new fraction: (num*t1 + t2) / (t2*k)  (reduced later) */
        seq_t new_den = mul_small(t2, (unsigned)k);       /* t2 * k */
        seq_t prod    = mul_small(num, (unsigned)t1);     /* num * t1 */
        seq_t new_num = prod + t2;                        /* num*t1 + t2 */

        /* reduce the fraction */
        seq_t gg = gcd(new_num, new_den);
        num = new_num / gg;
        den = new_den / gg;
    }

    return num;                /* numerator of H_{2n} in lowest terms */
}
