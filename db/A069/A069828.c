/*
 * A069828  Sum of positive integers k for k <= n and gcd(k,n) = gcd(k+1,n).
 * https://oeis.org/A069828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)/2 * Σ_{d|n} mu(d) * tau(d) * (n/d) (n > 1), a(1) =
 *            1. The sum is taken over all positive divisors d of n. μ(d) is
 *            the Möbius function; τ(d) the divisor‑count function
 * Range:     u8 n <= 28, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   enumerate_divisors, factor_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_uint64.h"
#include "../utils/enumerate_divisors.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    /* Factor n into primes and exponents */
    uint64_t fac[20];
    int        exp[20];
    int        cnt = 0;
    factor_uint64(n, fac, exp, &cnt);

    seq_t posSum = 0;
    seq_t negSum = 0;

    enumerate_divisors(n, fac, exp, cnt, 0, 1, 1, 1, &posSum, &negSum);

    /* B = Σ μ(d)*τ(d)*(n/d).  It must be non‑negative for n>1. */
    seq_t B = posSum;
    if (negSum > posSum)
        return 0;                 /* safety, should not happen */
    B -= negSum;

    /* a(n) = (n-1)/2 * B */
    seq_t factor = (seq_t)(n - 1) / 2;
    return factor * B;
}
