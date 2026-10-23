/*
 * A195585  sigma(2*n^2) - sigma(n^2).
 * https://oeis.org/A195585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = σ(2·n²) – σ(n²) (OEIS A195585)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute base^exp for integer exponents (seq_t arithmetic). */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}

/* Return the n-th term of sequence A195585. */
seq_t a(idx_t n)
{
    if (n == 0)        /* offset is 1; return 0 for safety */
        return 0;

    /* Factor n: store prime factors and exponents. */
    struct { idx_t p; idx_t e; } fac[64];   /* enough for 64‑bit n */
    size_t fc = 0;
    idx_t tmp = n;
    idx_t two_exp = 0;              /* exponent of 2 in n (0 if absent) */

    /* Handle factor 2 first. */
    if ((tmp & 1) == 0)
    {
        idx_t e = 0;
        while ((tmp & 1) == 0)
        {
            tmp >>= 1;
            ++e;
        }
        two_exp = e;
        fac[fc].p = 2;
        fac[fc].e = e;
        ++fc;
    }

    /* Handle odd primes */
    for (idx_t p = 3; p <= tmp / p; p += 2)
    {
        if (tmp % p == 0)
        {
            idx_t e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            fac[fc].p = p;
            fac[fc].e = e;
            ++fc;
        }
    }
    if (tmp > 1)            /* remaining prime factor */
    {
        fac[fc].p = tmp;
        fac[fc].e = 1;
        ++fc;
    }

    /* Compute σ(n²) and σ(2·n²) multiplicatively. */
    seq_t sigma_n2  = 1;
    seq_t sigma_2n2 = 1;

    for (size_t i = 0; i < fc; ++i)
    {
        idx_t p   = fac[i].p;
        idx_t e   = fac[i].e;
        idx_t exp_n2  = 2 * e;                /* exponent in n²    */
        seq_t term_n2 = (pow_seq((seq_t)p, exp_n2 + 1) - 1) / (p - 1);
        sigma_n2 *= term_n2;

        idx_t exp_2n2 = (p == 2) ? (2 * e + 1) : exp_n2;    /* exponent in 2·n² */
        seq_t term_2n2 = (pow_seq((seq_t)p, exp_2n2 + 1) - 1) / (p - 1);
        sigma_2n2 *= term_2n2;
    }

    /* If 2 was not a factor of n, include its contribution for 2·n². */
    if (two_exp == 0)
        sigma_2n2 *= 3;            /* σ(2¹) = 1 + 2 = 3 */

    return sigma_2n2 - sigma_n2;
}
