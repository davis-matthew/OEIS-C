/*
 * A209187  Sum of divisors of n minus cototient of n.
 * https://oeis.org/A209187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = σ(n) + φ(n) – n (sum of divisors of n minus the cototient
 *            of n)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation is performed by trial division.
   It is accurate for all n ≥ 1 and requires only O(√n) time
   per call, which is well below the limits for n ≤ 10 000. */

#include <stdint.h>

/* Helper: compute σ(n) – the sum of all divisors of n */
static seq_t sigma_of(idx_t n)
{
    seq_t sigma = 1;                 /* multiplicative identity */
    idx_t m = n;

    /* handle factor 2 separately for speed */
    if (m % 2 == 0) {
        seq_t p_pow = 1;
        int exp = 0;
        while (m % 2 == 0) {
            m /= 2;
            exp++;
            p_pow *= (seq_t)2;
        }
        /* term = (2^(exp+1) – 1)/(2 – 1) = 2^(exp+1) – 1 */
        sigma *= (p_pow * (seq_t)2 - 1);
    }

    /* odd factors */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            seq_t p_pow = 1;
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
                p_pow *= (seq_t)p;
            }
            /* term = (p^(exp+1) – 1)/(p – 1) */
            seq_t term = (p_pow * (seq_t)p - 1) / ((seq_t)p - 1);
            sigma *= term;
        }
    }

    /* if a prime factor > sqrt(n) remains */
    if (m > 1) {
        /* exponent is 1, term = (m^2 – 1)/(m – 1) = m + 1 */
        sigma *= ((seq_t)m * (seq_t)m - 1) / ((seq_t)m - 1);
    }

    return sigma;
}

/* Helper: compute φ(n) – Euler's totient function of n */
static seq_t phi_of(idx_t n)
{
    seq_t phi = (seq_t)n;
    idx_t m = n;

    if (m % 2 == 0) {
        phi -= phi / 2;
        while (m % 2 == 0) m /= 2;
    }

    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            phi -= phi / p;
            while (m % p == 0) m /= p;
        }
    }

    if (m > 1) phi -= phi / m;

    return phi;
}

/* Main requested function */
seq_t a(idx_t n)
{
    /* n ≥ 1 by problem statement */
    seq_t sigma = sigma_of(n);
    seq_t phi   = phi_of(n);
    return sigma + phi - (seq_t)n;
}
