/*
 * A029141  Expansion of 1/((1-x^2)(1-x^3)(1-x^4)(1-x^11)).
 * https://oeis.org/A029141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            floor((198*(-1+(-1)^n)*(-1)^((n-1)*n/2)+(n+10)*(2*n^2+40*n+125+99*(-1)^n)+1216)/3168)
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to signed long long for intermediate calculations.
       The values involved for n up to typical limits fit in 64‑bit. */
    long long nn = (long long)n;

    /* (-1)^n:  1 for even n, -1 for odd n */
    long long sign1 = (nn & 1) ? -1 : 1;

    /* (-1)^((n-1)*n/2):  -1 when n mod 4 is 2 or 3, else +1 */
    long long sign2 = (nn & 3) >= 2 ? -1 : 1;

    /* Compute the numerator of the rational expression. */
    long long term1 = 198 * (-1 + sign1) * sign2;   /* 198*(-1+(-1)^n)*(-1)^((n-1)*n/2) */
    long long term2 = (nn + 10) *
                      ( 2 * nn * nn + 40 * nn + 125 + 99 * sign1 ); /* (n+10)*(...) */
    long long numerator = term1 + term2 + 1216;

    /* Denominator */
    const long long denom = 3168;

    /* Floor division: C truncates toward zero, so adjust for negative numerators. */
    long long result;
    if (numerator >= 0)
        result = numerator / denom;
    else
        result = (numerator - (denom - 1)) / denom;   /* floor for negative */

    return (seq_t)result;
}
