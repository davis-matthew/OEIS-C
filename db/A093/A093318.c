/*
 * A093318  a(n) = number of positive divisors k of n where mu(k) = 1 and
 *          mu(n/k) = -1.
 * https://oeis.org/A093318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^{(τ₂(n) - 1)} if all prime exponents of n are ≤ 2 * and
 *            the number of primes with exponent exactly 1 (denoted τ₂(n)) *
 *            is odd; otherwise a(n) = 0. * * Here τ₂(n) counts the primes p
 *            dividing n with exp(p,n) = 1. * * The function factors n by
 *            trial division; for the limits of the * requir...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Simple trial‑division factorisation.  Results are returned in parallel
 * arrays pr[] (prime) and exp[] (exponent).  cnt contains the number of
 * distinct primes found.  Arrays must be at least 20 elements long,
 * which is safe for n ≤ 10 000. */
static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    *cnt = 0;
    if (n < 2) return;

    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            uint64_t e = 0;
            pr[*cnt] = p;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            exp[*cnt] = e;
            ++(*cnt);
        }
    }
    if (n > 1)
    {
        pr[*cnt] = n;
        exp[*cnt] = 1;
        ++(*cnt);
    }
}

seq_t a(idx_t n)
{
    /* Ensure we only work with unsigned values. */
    uint64_t N = (uint64_t)n;

    if (N < 1) return (seq_t)0;  /* offset starts at 1 */

    /* Factor n.  Use 20 slots: safe for n up to 10 000. */
    uint64_t pr[20];
    uint64_t exp[20];
    size_t  cnt;
    factor_u64(N, pr, exp, &cnt);

    /* Count primes with exponent 1 (τ₂(n)).  Reject if any exp > 2. */
    int tau1 = 0;            /* number of primes with exponent exactly 1 */
    for (size_t i = 0; i < cnt; ++i)
    {
        if (exp[i] > 2) return (seq_t)0;
        if (exp[i] == 1) ++tau1;
    }

    /* Need τ₂(n) to be odd; otherwise no divisor satisfies both
     * mobius conditions. */
    if ((tau1 & 1) == 0) return (seq_t)0;

    /* Number of admissible divisors = 2^(τ₂(n)-1).  τ₂(n)≥1 here. */
    return ((seq_t)1) << (tau1 - 1);
}
