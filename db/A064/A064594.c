/*
 * A064594  Nonunitary multiply perfect numbers: the sum of the nonunitary
 *          divisors of n is a multiple of n; i.e., n divides sigma(n) -
 *          usigma(n).
 * https://oeis.org/A064594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th positive integer m such that m divides σ(m) –
 *            usσ(m), where σ is the sum of all divisors and usσ is the sum of
 *            the unitary divisors
 * Range:     u8 n <= 11, u16 n <= 159, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether m satisfies m | (σ(m) – usσ(m)). */
static bool good(idx_t m)
{
    seq_t sigma   = 1;
    seq_t usigma  = 1;
    idx_t tmp     = m;

    /* Factorise m using trial division. */
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            idx_t e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }

            /* p^e */
            seq_t pow_pe = 1;
            for (idx_t i = 0; i < e; ++i)
                pow_pe *= (seq_t)p;

            seq_t pow_pe1 = pow_pe * (seq_t)p;                /* p^(e+1) */

            /* σ(p^e) = (p^(e+1) – 1)/(p – 1) */
            seq_t sigma_fac = (pow_pe1 - 1) / ((seq_t)p - 1);
            sigma *= sigma_fac;

            /* usσ(p^e) = 1 + p^e */
            usigma *= (1 + pow_pe);
        }
    }

    /* Remaining prime factor (if any). */
    if (tmp > 1)
    {
        idx_t p = tmp;
        seq_t pow_pe = (seq_t)p;
        seq_t pow_pe1 = pow_pe * (seq_t)p;
        seq_t sigma_fac = (pow_pe1 - 1) / ((seq_t)p - 1);
        sigma *= sigma_fac;
        usigma *= (1 + pow_pe);
    }

    /* Check the divisibility condition. */
    seq_t diff = sigma - usigma;
    return (diff % (seq_t)m) == 0;
}

/* External function required by the OEIS harness. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;             /* safeguard for offset 1 */

    idx_t count    = 0;              /* how many good numbers found */
    idx_t candidate = 1;             /* next integer to test */

    while (true)
    {
        if (good(candidate))
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
