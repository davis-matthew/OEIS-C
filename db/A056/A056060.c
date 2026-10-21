/*
 * A056060  The powerfree part of the central binomial coefficients.
 * https://oeis.org/A056060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A055231(A001405(n)) / GCD(A055231(A001405(n)), largest
 *            square divisor of A001405(n)) which is the product of all primes
 *            p such that p appears to the exact power 1 in the central
 *            binomial coefficient C(n,⌊n/2⌋)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 38, u64 n <= 70, u128 n <= 150,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes.h"   /* provides:  void build_primes(idx_t limit, idx_t **out, size_t *cnt); */

static idx_t *primes = NULL;          /* array of primes up to max_prime_limit */
static size_t prime_count = 0;        /* number of primes in the array */
static idx_t max_prime_limit = 0;     /* largest n for which the prime list is valid */

/* Ensure that the prime list contains all primes ≤ limit. */
static void ensure_primes(idx_t limit)
{
    if (limit <= max_prime_limit) return;
    free(primes);
    build_primes(limit, &primes, &prime_count);
    max_prime_limit = limit;
}

/* Count exponent of prime p in m! using Legendre's formula. */
static idx_t fact_exp(idx_t m, idx_t p)
{
    idx_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for 0, but safe */
    idx_t k = n >> 1;              /* floor(n/2) */

    ensure_primes(n);

    seq_t result = 1;
    for (size_t i = 0; i < prime_count && primes[i] <= n; ++i) {
        idx_t p = primes[i];
        idx_t e = fact_exp(n, p) - fact_exp(k, p) - fact_exp(n - k, p);
        if (e == 1) {
            result *= (seq_t)p;
        }
    }
    return result;
}
