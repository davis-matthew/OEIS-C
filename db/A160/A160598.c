/*
 * A160598  Numerator of coresilience C(n) = (n - phi(n))/(n-1).
 * https://oeis.org/A160598
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = the reduced numerator of (n - φ(n))/(n-1)
 * Range:     u8 n <= 359, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static inline seq_t phi_of(idx_t n)
{
    seq_t phi = (seq_t)n;
    idx_t tmp = n;

    /* trial division for prime factors up to sqrt(tmp) */
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            /* remove all occurrences of p from tmp */
            while (tmp % p == 0)
                tmp /= p;
            /* φ(n) = φ(n)/p * (p-1) for each distinct prime factor p */
            seq_t p_seq = (seq_t)p;
            phi = phi / p_seq * (p_seq - 1);
        }
    }

    /* remaining prime factor greater than sqrt(original n) */
    if (tmp > 1)
    {
        seq_t p_seq = (seq_t)tmp;
        phi = phi / p_seq * (p_seq - 1);
    }

    return phi;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* undefined below offset – harmless */

    seq_t n_seq = (seq_t)n;
    seq_t phi = phi_of(n);

    seq_t num = n_seq - phi;          /* numerator before reduction */
    seq_t den = n_seq - 1;            /* denominator            */

    seq_t g = gcd(num, den);          /* reduce the fraction    */
    return num / g;                   /* reduced numerator      */
}
