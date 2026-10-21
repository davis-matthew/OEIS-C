/*
 * A081238  #{(i,j): mu(i)*mu(j) = -1, 1 <= i <= n, 1 <= j <= n}, where
 *          mu=A008683 (Moebius function).
 * https://oeis.org/A081238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * (#i ≤ n with μ(i) = +1) * (#j ≤ n with μ(j) = –1)
 * Range:     u8 n <= 34, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ≡ 2 × (number of square‑free integers ≤ n with an even number of
   prime factors) × (number of square‑free integers ≤ n with an odd
   number of prime factors)                     */

#include <stdbool.h>
#include "../utils/mobius.h"   /* needed only for the type definitions */

seq_t a(idx_t n)
{
    /* Count μ(i)=+1 and μ(i)=–1 for 1 ≤ i ≤ n by a simple sieve. */
    if (n == 0)   /* the sequence is defined for n ≥ 1, but guard anyway */
        return (seq_t)0;

    /* Allocate temporary arrays.  For n ≤ 100 000 this is tiny (<1 kB). */
    int8_t *mu = (int8_t*)malloc((n + 1) * sizeof(int8_t));
    bool   *is_prime = (bool*)malloc((n + 1) * sizeof(bool));
    if (!mu || !is_prime) {
        /* Memory allocation failed – return 0 as a safe fallback. */
        free(mu);
        free(is_prime);
        return (seq_t)0;
    }

    /* initialise μ(i) = 1 for all i; μ(0) unused */
    for (idx_t i = 1; i <= n; ++i)
        mu[i] = 1;

    /* sieve for primes up to n */
    for (idx_t i = 0; i <= n; ++i)
        is_prime[i] = true;
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= n; j += p)
                is_prime[j] = false;
        }
    }

    /* apply Möbius transformations */
    for (idx_t p = 2; p <= n; ++p) {
        if (is_prime[p]) {
            /* multiply by –1 on multiples of p */
            for (idx_t k = p; k <= n; k += p)
                mu[k] = (int8_t)(-mu[k]);

            /* zero on multiples of p² */
            idx_t sq = p * p;
            if (sq <= n)
                for (idx_t k = sq; k <= n; k += sq)
                    mu[k] = 0;
        }
    }

    /* Count +1 and –1 values */
    seq_t pos = 0, neg = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (mu[i] == 1)
            ++pos;
        else if (mu[i] == -1)
            ++neg;
    }

    free(mu);
    free(is_prime);

    return (seq_t)2 * pos * neg;
}
