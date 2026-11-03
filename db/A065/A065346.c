/*
 * A065346  a(n) = binomial(2*n, n) mod ((n+1)*(n+2)*(n+3)*(n+4)).
 * https://oeis.org/A065346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n, n) mod ((n+1)*(n+2)*(n+3)*(n+4))
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 266, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 52 terms through n = 9087 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute binomial(2n,n) modulo M using prime‑factorisation
   (exponents are calculated with Legendre’s formula). */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"

/* simple sieve: generate all primes up to limit,
   returns array of primes (malloced) and their count. */
static void generate_primes(uint32_t limit, uint32_t **primes, size_t *cnt)
{
    if (limit < 2) {
        *primes = NULL;
        *cnt = 0;
        return;
    }
    bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_prime) {
        *primes = NULL;
        *cnt = 0;
        return;
    }
    memset(is_prime, 1, limit + 1);
    is_prime[0] = is_prime[1] = 0;
    for (uint32_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }
    *cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (is_prime[i]) (*cnt)++;
    *primes = (uint32_t *)malloc(*cnt * sizeof(uint32_t));
    if (!*primes) {
        free(is_prime);
        *cnt = 0;
        return;
    }
    size_t k = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (is_prime[i]) (*primes)[k++] = i;
    free(is_prime);
}

/* ---------- helpers ------------------------------------- */

/* integer exponent of prime p in n! (Legendre) */
static uint64_t exp_in_factorial(uint64_t n, uint32_t p)
{
    uint64_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/* ---------- sequence function -------------------------------- */

seq_t a(idx_t n)
{
    /* modulus M = (n+1)*(n+2)*(n+3)*(n+4) */
    seq_t M = (seq_t)(n + 1);
    M *= (seq_t)(n + 2);
    M *= (seq_t)(n + 3);
    M *= (seq_t)(n + 4);
    if (M == 0)          /* defensive, should not happen for tested n */
        return 0;

    uint32_t limit = (uint32_t)(2 * (uint64_t)n);
    uint32_t *primes = NULL;
    size_t primecnt = 0;
    generate_primes(limit, &primes, &primecnt);

    seq_t result = 1 % M;
    for (size_t i = 0; i < primecnt; ++i) {
        uint32_t p = primes[i];
        uint64_t e = exp_in_factorial(limit, p) - 2 * exp_in_factorial((uint64_t)n, p);
        if (e == 0)
            continue;
        seq_t term = powmod((seq_t)p, e, M);
        result = (unsigned __int128)result * term % M;
    }

    free(primes);
    return result;
}
