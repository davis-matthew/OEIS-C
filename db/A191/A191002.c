/*
 * A191002  Completely multiplicative function with a(prime(k)) =
 *          prime(k)*prime(k+1).
 * https://oeis.org/A191002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n * ∏_{p|n} next_prime(p)^{v_p(n)} (completely
 *            multiplicative, a(prime_k) = prime_k * prime_{k+1})
 * Range:     u8 n <= 11, u16 n <= 95, u32 n <= 6143, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/next_prime.h"  /* next_prime(uint64_t start) */

seq_t a(idx_t n)
{
    /* Undefined for n == 0; return 0 as a harmless default */
    if (n == 0)
        return 0;

    /* Initialize result with the factor n itself. */
    seq_t result = (seq_t)n;

    /* Factor n into primes and exponents.  The arrays are sized
       generously for the expected maximum exponent count. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   count = 0;
    factor_u64((uint64_t)n, primes, exps, &count);

    /* For each distinct prime factor p with multiplicity e, multiply
       by the e‑th power of the succeeding prime.  next_prime(p)
       returns the smallest prime strictly larger than p. */
    for (size_t i = 0; i < count; ++i) {
        uint64_t next_p = next_prime(primes[i]);          /* p_{k+1} */
        seq_t     factor = (seq_t)next_p;
        uint64_t  exp   = exps[i];
        while (exp--) {
            result *= factor;
        }
    }

    return result;
}
