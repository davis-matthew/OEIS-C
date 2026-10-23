/*
 * A120031  Numerators of reduced forms of fractions obtained by performing
 *          the first n divisions shown below.
 * https://oeis.org/A120031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the reduced fraction x(n), where x(0) = 1,
 *            x(2n+1) = x(2n) * (4n+1)/(4n+2), x(2n+2) = x(2n+1) *
 *            (4n+4)/(4n+3). The recurrence is followed step by step, reducing
 *            after each multiplication to keep the numbers minimal
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0u)
        return 1;

    seq_t num = 1;   /* numerator of x(k) */
    seq_t den = 1;   /* denominator of x(k) */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t nf, df;   /* numerator and denominator factors for step k */

        if (k & 1u)    /* odd k = 2m+1 */
        {
            idx_t m = (k - 1u) >> 1;
            nf = (seq_t)(4 * m + 1);
            df = (seq_t)(4 * m + 2);
        }
        else           /* even k = 2m+2 */
        {
            idx_t m = (k - 2u) >> 1;
            nf = (seq_t)(4 * m + 4);
            df = (seq_t)(4 * m + 3);
        }

        /* Reduce factors with current fraction to keep numbers small */
        seq_t g1 = gcd(num, df);
        num /= g1;  df /= g1;

        seq_t g2 = gcd(den, nf);
        den /= g2;  nf /= g2;

        /* Multiply reduced factors */
        seq_t new_num = num * nf;
        seq_t new_den = den * df;

        /* Final reduction */
        seq_t g3 = gcd(new_num, new_den);
        num = new_num / g3;
        den = new_den / g3;
    }

    return num;
}
