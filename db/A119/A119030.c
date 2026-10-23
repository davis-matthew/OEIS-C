/*
 * A119030  Difference between numerator and denominator of the sum of all
 *          matrix elements of n X n Hilbert matrix M(i,j)=1/(i+j-1) (i,j =
 *          1..n), A117731[n] - A117664[n].
 * https://oeis.org/A119030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Numerator[Σ_{i=1}^{n} Σ_{j=1}^{n} 1/(i+j-1)] -
 *            Denominator[Σ_{i=1}^{n} Σ_{j=1}^{n} 1/(i+j-1)]
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  using the identity S(n) = n + Σ_{k=n+1}^{2n-1} (2n-k)/k             */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Sum of diagonal elements (each is 1) */
    seq_t num = (seq_t)n;     /* numerator after simplification */
    seq_t den = (seq_t)1;     /* denominator after simplification */

    /* Add the remaining entries grouped by the same denominator k  */
    idx_t two_n = n + n;               /* 2n  (no overflow for n up to 10 000) */

    for (idx_t k = n + 1; k <= two_n - 1; ++k)
    {
        seq_t m = (seq_t)(two_n - k);  /* multiplicity of 1/k in the sum  */
        seq_t d = (seq_t)k;            /* current denominator          */

        /* (num/den) + (m/d)  ->  (num*d + den*m) / (den*d)  */
        seq_t new_num = num * d + den * m;
        seq_t new_den = den * d;

        /* Reduce the fraction */
        seq_t g = gcd(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    return num - den;
}
