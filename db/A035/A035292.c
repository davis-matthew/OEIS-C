/*
 * A035292  Number of similar sublattices of Z^4 of index n^2.
 * https://oeis.org/A035292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers of n of * a(p^e) where * a(2^e)
 *            = 3, * a(p^e) = (e+1)*p^e + (2*(1+(e*p-e-1)*p^e))/((p-1)^2) for
 *            odd p. * The function is multiplicative and works for all n ≥ 1
 * Range:     u8 n <= 12, u16 n <= 1889, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;                  /* a(1) = 1 */

    seq_t result = 1;
    idx_t m = n;

    /* factor out the prime 2 */
    idx_t e2 = 0;
    while (m % 2 == 0) {
        m /= 2;
        ++e2;
    }
    if (e2 > 0)
        result *= (seq_t)3;                /* a(2^e) = 3 for e>=1 */

    /* trial division for odd primes */
    for (idx_t p = 3; p <= m / p; ) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            /* compute p^e as seq_t */
            seq_t pow_pe = 1;
            for (idx_t k = 0; k < e; ++k)
                pow_pe *= (seq_t)p;

            /* A = (e+1)*p^e */
            seq_t A = (seq_t)(e + 1) * pow_pe;

            /* temp = (e*p - e - 1)*p^e */
            seq_t temp = (((seq_t)e * (seq_t)p) - (seq_t)e - (seq_t)1) * pow_pe;

            /* numerator = 2*(1+temp) */
            seq_t numerator = 2 * ((seq_t)1 + temp);

            /* denom = (p-1)^2 */
            seq_t denom = ((seq_t)p - (seq_t)1) * ((seq_t)p - (seq_t)1);

            /* B = numerator / denom (exact) */
            seq_t B = numerator / denom;

            seq_t a_pe = A + B;

            result *= a_pe;
        }

        /* next odd candidate */
        p += 2;
    }

    /* if remaining factor > 1, it is a prime with exponent 1 */
    if (m > 1) {
        idx_t p = m;
        seq_t pow_pe = (seq_t)p;          /* p^1 */

        /* A = (1+1)*p^1 = 2p */
        seq_t A = (seq_t)2 * pow_pe;

        /* temp = (p - 1 - 1)*p = (p-2)p */
        seq_t temp = ((seq_t)p - (seq_t)2) * pow_pe;

        /* numerator = 2*(1+temp) = 2*(1+(p-2)p) */
        seq_t numerator = 2 * ((seq_t)1 + temp);

        /* denom = (p-1)^2 */
        seq_t denom = ((seq_t)p - (seq_t)1) * ((seq_t)p - (seq_t)1);

        seq_t B = numerator / denom;

        seq_t a_pe = A + B;

        result *= a_pe;
    }

    return result;
}
