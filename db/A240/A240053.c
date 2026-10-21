/*
 * A240053  3rd arithmetic derivation of products of 2 successive prime
 *          numbers (A006094).
 * https://oeis.org/A240053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A006094(n))'''
 * Range:     u8 n <= 15, u16 n <= 268, u32 n <= 429, u64 n <= 429,
 *            u128 n <= 429, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 3rd arithmetic derivative of the product of the n‑th and (n+1)-th primes. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/* --------------------------------------------------------------------- */
/*  Prime sieve – build a static list of primes up to a fixed limit      */
/* --------------------------------------------------------------------- */
static uint32_t *primes = NULL;
static size_t   prime_count = 0;

/* generate all primes ≤ LIMIT, 1‑based indexing is not necessary for us */
static void init_primes(void)
{
    if (prime_count > 0) return;              /* already initialised */

    /* Upper bound chosen to comfortably cover the first 10 000 primes. */
    const uint64_t LIMIT = 2000000ULL;
    bool *sieve = (bool *)malloc(LIMIT + 1);
    if (!sieve) abort();
    for (uint64_t i = 0; i <= LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (uint64_t i = 2; i * i <= LIMIT; ++i)
        if (sieve[i])
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = false;

    /* Count primes and allocate */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (sieve[i]) ++cnt;
    primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!primes) abort();
    prime_count = cnt;

    size_t idx = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i)
        if (sieve[i]) primes[idx++] = (uint32_t)i;

    free(sieve);
}

/* --------------------------------------------------------------------- */
/*  Arithmetic derivative for uint64_t values                            */
/* --------------------------------------------------------------------- */
static uint64_t derivative_u64(uint64_t m)
{
    if (m <= 1) return 0;                      /* d(0) = d(1) = 0 */

    uint64_t result = 0;
    uint64_t temp   = m;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            uint32_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            result += (m / p) * exp;
        }
    }
    if (temp > 1) {                            /* remaining prime factor */
        result += (m / temp);                  /* exponent = 1 */
    }
    return result;
}

/* --------------------------------------------------------------------- */
/*  Sequence function                                                     */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                       /* undefined, but harmless */

    init_primes();                              /* ensure primes ready */

    /* We need the first (n+1) primes. */
    assert((size_t)n + 1 <= prime_count);

    uint64_t p1 = (uint64_t)primes[(size_t)n - 1];
    uint64_t p2 = (uint64_t)primes[(size_t)n];

    uint64_t prod  = p1 * p2;                   /* A006094(n)            */
    uint64_t d1    = derivative_u64(prod);      /* first arithmetic deriv */
    uint64_t d2    = derivative_u64(d1);        /* second                */
    uint64_t d3    = derivative_u64(d2);        /* third                 */

    return (seq_t)d3;
}
