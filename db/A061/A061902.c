/*
 * A061902  Number of digits in n-th term of A061482.
 * https://oeis.org/A061902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = A055642(A061482(n)) = number of decimal digits of b(n),
 *            where b(1)=1, b(2)=2, b(3)=3, b(n+1) = b(n)^2 + P(n-1) for n>=3,
 *            P(k) = b(1)*...*b(k). Computed with logarithms: ln b(n+1) = 2 ln
 *            b(n) + log1p(P(n-1)/b(n)^2)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 3) return (seq_t)1;
    const long double LN10 = 2.302585092994045684017991454684364L;
    long double lb = logl(3.0L);   /* ln b(3) */
    long double lp = logl(2.0L);   /* ln P(2) = ln(1*2) */
    for (idx_t k = 3; k < n; k++) {
        /* b(k+1) = b(k)^2 + P(k-1) */
        long double e = lp - 2.0L * lb;
        long double r = (e < -700.0L) ? 0.0L : expl(e);
        long double nb = 2.0L * lb + log1pl(r);
        lp += lb;      /* P(k) = P(k-1) * b(k) */
        lb = nb;
    }
    long long d = (long long)(lb / LN10) + 1;
    return (seq_t)d;
}
