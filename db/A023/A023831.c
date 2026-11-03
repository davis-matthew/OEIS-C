/*
 * A023831  Sum of exponents in prime-power factorization of C(4n,n+1).
 * https://oeis.org/A023831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_p [ v_p((4n)!) – v_p((n+1)!) – v_p((3n-1)!) ]
 * Range:     u8 n <= 749, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 8436 (2026-11-01)
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where v_p(m!) = Σ_{i≥1} floor(m/p^i) is the exponent of prime p in m! */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/sieve_primes.h"

/* Compute Σ_p v_p(m!) for all primes p ≤ m using Legendre's formula. */
static seq_t
bigomega_factorial(idx_t m, const idx_t *primes, size_t primes_cnt)
{
    seq_t sum = 0;
    for (size_t i = 0; i < primes_cnt && primes[i] <= m; ++i) {
        idx_t p = primes[i];
        idx_t t = m / p;
        while (t) {
            sum += t;
            t /= p;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* a(1) is defined; for n==0 we return 0 to keep the function safe. */
    if (n == 0) return 0;

    /* Get all primes up to 4n once per call. */
    idx_t *primes = NULL;
    size_t prime_cnt = 0;
    sieve_primes(4 * n, &primes, &prime_cnt);

    /* Sum of exponents of primes in the factorials. */
    seq_t e4 = bigomega_factorial(4 * n, primes, prime_cnt);
    seq_t e2 = bigomega_factorial(n + 1, primes, prime_cnt);
    seq_t e3 = bigomega_factorial(3 * n - 1, primes, prime_cnt);

    /* Result is the difference of the three sums. */
    seq_t result = e4 - e2 - e3;

    free(primes);
    return result;
}
