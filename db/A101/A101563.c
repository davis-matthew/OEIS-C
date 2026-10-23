/*
 * A101563  a(n) = (-1)^n * coefficient of x^n in Sum_{k>=1}
 *          x^(k-1)/(1+10*x^k).
 * https://oeis.org/A101563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (-1)^n * Sum_{d|n+1} (-10)^(d-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    idx_t m = n + 1;          /* m = n + 1 */
    seq_t pos = 0;            /* sum of terms with positive sign */
    seq_t neg = 0;            /* sum of terms with negative sign */

    /* Enumerate all divisors d of m */
    for (idx_t i = 1; i <= m / i; ++i)
    {
        if (m % i == 0)
        {
            idx_t d = i;
            seq_t term = pow10(d - 1);
            if (((n + d - 1) & 1ULL) == 0)
                pos += term;   /* positive sign */
            else
                neg += term;   /* negative sign */

            if (i != m / i)    /* second divisor, if distinct */
            {
                d = m / i;
                term = pow10(d - 1);
                if (((n + d - 1) & 1ULL) == 0)
                    pos += term;
                else
                    neg += term;
            }
        }
    }
    return pos - neg;          /* a(n) is guaranteed non‑negative */
}
