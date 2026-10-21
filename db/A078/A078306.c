/*
 * A078306  a(n) = Sum_{d divides n} (-1)^(n/d+1)*d^2.
 * https://oeis.org/A078306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy, changed
 * Formula:   a(n) = Sum_{d|n} (-1)^(n/d+1) * d^2 This sequence is
 *            multiplicative with a(2^e) = (2*4^e+1)/3 a(p^e) =
 *            (p^(2e+2)-1)/(p^2-1) for an odd prime p. The following
 *            implementation factors n by trial division and evaluates the
 *            above formula for each prime power. It works for any build where
 *            seq_t and idx...
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 2148, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast exponentiation of seq_t values. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0)
    {
        if (exp & 1U)
            result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

/* Compute the nth term of OEIS A078306.  The offset is 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n=0, return 0 */

    idx_t remaining = n;
    seq_t result   = 1;

    /* Handle the power of 2 separately. */
    idx_t e2 = 0;
    while ((remaining & 1ULL) == 0ULL)
    {
        ++e2;
        remaining >>= 1;
    }
    if (e2 > 0)
    {
        seq_t pow4e = pow_seq_t((seq_t)4, e2);          /* 4^e2 */
        seq_t term2 = ((seq_t)2 * pow4e + (seq_t)1) / (seq_t)3;
        result *= term2;
    }

    /* Factor odd primes. */
    for (idx_t p = 3; p * p <= remaining; p += 2)
    {
        if (remaining % p == 0)
        {
            idx_t e = 0;
            while (remaining % p == 0)
            {
                ++e;
                remaining /= p;
            }
            seq_t pow_term = pow_seq_t((seq_t)p, 2 * e + 2);     /* p^(2e+2) */
            seq_t numerator   = pow_term - (seq_t)1;
            seq_t denom       = (seq_t)p * (seq_t)p - (seq_t)1;  /* p^2 - 1 */
            seq_t term = numerator / denom;
            result *= term;
        }
    }

    /* If a prime factor > sqrt(n) remains, it is a single power of 1. */
    if (remaining > 1)
    {
        seq_t pow_term = pow_seq_t((seq_t)remaining, 2 * 1 + 2); /* p^4 */
        seq_t numerator   = pow_term - (seq_t)1;
        seq_t denom       = (seq_t)remaining * (seq_t)remaining - (seq_t)1;
        seq_t term = numerator / denom;
        result *= term;
    }

    return result;
}
