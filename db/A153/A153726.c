/*
 * A153726  Initial digit of Catalan number A000108(n).
 * https://oeis.org/A153726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A000030(A000108(n)) (first decimal digit of Catalan
 *            number)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compute the first decimal digit of the Catalan number C_n.
   For n==0 the Catalan number is 1, whose first digit is 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;               /* C_0 = 1 */

    /* Precompute the constant ln(10) for conversion from natural
       logarithm (returned by lgammal) to base‑10 logarithm. */
    static const long double LN10 = logl(10.0L);

    /* log10(n!) = lgamma(n+1) / ln(10) */
    long double logfact_n   = lgammal((long double)n + 1.0L) / LN10;
    long double logfact_2n  = lgammal((long double)2 * n + 1.0L) / LN10;

    /* log10(C_n) = log10((2n)!) - 2*log10(n!) - log10(n+1) */
    long double logC = logfact_2n - 2.0L * logfact_n - log10l((long double)(n + 1));

    long double fractional = logC - floorl(logC);
    long double p10 = powl(10.0L, fractional);

    /* Protect against tiny numerical noise that could lead to a result
       just below an exact integer. */
    int digit = (int)(p10 + 1e-12L);
    if (digit <= 0)
        digit = 1;

    return (seq_t)digit;
}
