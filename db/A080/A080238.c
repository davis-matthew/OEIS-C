/*
 * A080238  Largest squarefree number dividing sum of cubes of divisors of n.
 * https://oeis.org/A080238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007947( sigma_3(n) ) = product of distinct prime factors
 *            of σ₃(n)
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 303, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_sqrt, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_k.h"
#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"

static bool primes_built = false;
static uint64_t *primes = NULL;
static size_t prime_cnt = 0;

seq_t a(idx_t n)
{
    /* Build a list of primes once, up to 1 000 000, which is safe for
       all n ≤ 10 000 (the test standard). */
    if (!primes_built) {
        build_primes_upto(1000000, &primes, &prime_cnt);
        primes_built = true;
    }

    /* Sum of cubes of divisors of n (σ₃(n)) */
    seq_t sigma = sigma_k(n, 3);

    /* For n up to 10 000, σ₃(n) comfortably fits in 64 bit.  We
       cast to uint64_t for easier factorisation. */
    uint64_t sig_u = (uint64_t)sigma;
    if (sig_u == 0) return sigma;  /* defensive: should not occur */

    uint64_t rad = 1ULL;
    uint64_t remaining = sig_u;
    uint64_t sqrt_u = (uint64_t)floor_sqrt(sigma);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > sqrt_u) break;
        if (remaining % p == 0) {
            rad *= p;                      /* include prime once */
            while (remaining % p == 0)     /* strip all copies */
                remaining /= p;
        }
    }

    /* If anything remains > 1, it is a prime > sqrt_u */
    if (remaining > 1) rad *= remaining;

    return (seq_t)rad;
}
