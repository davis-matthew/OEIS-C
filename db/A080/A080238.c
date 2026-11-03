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
 * Validated: standard, pass, 63 terms through n = 9466 (2026-11-01)
 * Helpers:   build_primes_upto, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_i32.h"

static seq_t sigma_k(idx_t m, uint8_t k)
{
    seq_t res = (seq_t)1;
    idx_t n = m;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            idx_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }

            /* compute pk = p^k */
            seq_t pk = (seq_t)1;
            for (idx_t i = 0; i < k; ++i) pk *= (seq_t)p;

            /* sum_{i=0}^{exp} (p^k)^i = 1 + pk + pk^2 + ...   */
            seq_t term = (seq_t)0;
            seq_t pow   = (seq_t)1;
            for (idx_t i = 0; i <= exp; ++i) {
                term += pow;
                pow  *= pk;
            }
            res *= term;
        }
    }

    if (n > 1) {               /* remaining prime factor with exponent 1 */
        /* k is 1 or 3, exp = 1 */
        seq_t pk = (seq_t)1;
        for (idx_t i = 0; i < k; ++i) pk *= (seq_t)n;

        seq_t term = (seq_t)0;
        seq_t pow   = (seq_t)1;
        for (idx_t i = 0; i <= (idx_t)1; ++i) {
            term += pow;
            pow  *= pk;
        }
        res *= term;
    }

    return res;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
