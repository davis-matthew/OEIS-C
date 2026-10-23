/*
 * A132004  Expansion of (1 - phi(q^3) / phi(q) * phi(-q^2) * phi(-q^6)) / 2
 *          in powers of q where phi() is a Ramanujan theta function.
 * https://oeis.org/A132004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = multiplicative function with * a(2^e) = -1 (for all e ≥
 *            1) * a(3^e) = 1 * a(p^e) = e + 1 if p ≡ 1 (mod 4) * a(p^e) = (1
 *            + (-1)^e)/2 if p ≡ 3 (mod 4) * and a(1) = 1. The function is
 *            implemented by factorising n and * multiplying the prime‑power
 *            contributions. The implementation keeps * a sm...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   power_contrib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/power_contrib.h"

/* ----------  Small prime sieve  ------------------------------------- */
#define PRIME_SIEVE_LIMIT   1000000ULL
/* The primes will be stored in a dynamic array.  */
static uint64_t *prime_list = NULL;
static size_t    prime_count = 0;

/* Initialise the prime sieve only once.  */
static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* Allocate an array of bytes for the composite flags.  */
    uint8_t *is_comp = (uint8_t *)calloc(PRIME_SIEVE_LIMIT + 1, 1);
    if (!is_comp) return;                    /* Out‑of‑memory – abort. */

    size_t lim = PRIME_SIEVE_LIMIT;
    unsigned int i, j;
    for (i = 2; (uint64_t)i * i <= lim; ++i)
        if (!is_comp[i])
            for (j = i * i; j <= lim; j += i)
                is_comp[j] = 1;

    /* Count primes for allocation.  */
    prime_count = 0;
    for (i = 2; i <= lim; ++i)
        if (!is_comp[i]) ++prime_count;

    prime_list = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    if (!prime_list) { free(is_comp); return; }

    size_t idx = 0;
    for (i = 2; i <= lim; ++i)
        if (!is_comp[i])
            prime_list[idx++] = i;

    free(is_comp);
}

/* ----------  Main sequence function  -------------------------------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;     /* a(1) = 1 */

    init_primes();                          /* ensure primes are ready */

    uint64_t m = (uint64_t)n;
    seq_t    prod = (seq_t)1;

    for (size_t i = 0; i < prime_count; ++i)
    {
        uint64_t p = prime_list[i];
        if (p * p > m) break;
        if (m % p == 0)
        {
            uint64_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            prod *= power_contrib(p, e);
            if (prod == (seq_t)0) return (seq_t)0;      /* zero factor detected */
        }
    }

    /* If a prime factor remains > 1 */
    if (m > 1)
    {
        prod *= power_contrib(m, 1ULL);
    }

    return prod;
}
