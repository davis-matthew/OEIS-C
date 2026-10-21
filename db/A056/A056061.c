/*
 * A056061  Number of square divisors of central binomial coefficients.
 * https://oeis.org/A056061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of square divisors of the central binomial
 *            coefficient C(n,⌊n/2⌋)
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (i.e., a(n) = ∏_p (⌊e_p/2⌋+1) where e_p is the exponent of prime p in C(n,⌊n/2⌋)). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;     /* prime list, up to current limit */
static size_t prime_count = 0;      /* number of primes stored */
static idx_t prime_limit = 0;       /* largest n for which primes were generated */

/* Compute v_p(n!) using Legendre's formula */
static inline idx_t v_p_factorial(idx_t n, idx_t p)
{
    idx_t res = 0;
    idx_t div = p;
    while (div <= n) {
        res += n / div;
        if (div > n / p) break;   /* avoid overflow */
        div *= p;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;    /* safety for unexpected 0 */

    /* Ensure we have all primes up to n */
    if (n > prime_limit) {
        free(primes);
        build_primes_upto(n, &primes, &prime_count);
        prime_limit = n;
    }

    seq_t result = (seq_t)1;
    idx_t k = n / 2;               /* floor(n/2) */
    idx_t n_minus_k = n - k;

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = primes[i];
        if (p > n) break;          /* no further primes needed */

        idx_t e = v_p_factorial(n, p)
                - v_p_factorial(k, p)
                - v_p_factorial(n_minus_k, p);

        /* Contribution to the number of square divisors */
        seq_t factor = (seq_t)(e / 2 + 1);
        result *= factor;
    }

    return result;
}
