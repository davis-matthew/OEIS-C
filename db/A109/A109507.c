/*
 * A109507  Let x be a positive number, Lambda(d) = Moebius(d)*[log(x/d)]^2,
 *          f(m) = Sum_{d|m} Lambda(d), S(x) = Sum_{m <= x} f(m). Sequence
 *          gives nearest integer to S(n).
 * https://oeis.org/A109507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to Σ_{d=1}^n μ(d)·⌊n/d⌋·[log(n/d)]²
 * Range:     u8 n <= 40, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Be sure to link with a math library (-lm) because logl() is used */

#include <math.h>
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* defined only for n ≥ 1, return 0 for safety */

    long double sum = 0.0L;          /* accumulate in high‑precision floating point */
    for (idx_t d = 1; d <= n; ++d)
    {
        int mu = mobius(d);
        if (mu == 0) continue;          /* skip multiples of squares */

        idx_t q   = n / d;              /* floor(n/d) */
        long double r = (long double)n / (long double)d;
        long double l = logl(r);        /* natural log of n/d */
        long double term = (long double)mu * (long double)q * l * l;
        sum += term;
    }

    /* round to nearest integer and convert to unsigned 64‑bit integer */
    uint64_t res = (uint64_t)(sum + 0.5L);
    return (seq_t)res;                 /* cast to the appropriate seq_t type */
}
