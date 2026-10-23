/*
 * A014614  Numbers that are products of 5 primes (or 5-almost primes, a
 *          generalization of semiprimes).
 * https://oeis.org/A014614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 7150, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A014614 – 5-almost primes (products of exactly 5 prime factors counting multiplicity).
   The function computes the n‑th such number using a precomputed sieve up to 2,000,000.
   That range contains more than 300,000 terms, so n up to at least 10,000 is covered
   well within the time limit.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The build provides the integer types seq_t and idx_t. */
static const idx_t _sieve_limit = 2000000u;          /* upper bound for sieving */
static uint32_t *spf = NULL;                        /* smallest prime factor */
static uint8_t  *omega = NULL;                      /* big Omega (number of prime factors w/ multiplicity) */
static seq_t    *terms = NULL;                      /* list of 5‑almost primes */
static size_t    terms_count = 0;                    /* number of terms stored */
static bool      initialized = false;                /* flag to run the sieve only once */

/* -------------------------------------------------------------------------- */
static void
init_sieve(void)
{
    if (initialized) return;
    initialized = true;

    /* allocate arrays */
    spf   = (uint32_t *)calloc((size_t)_sieve_limit + 1, sizeof(uint32_t));
    omega = (uint8_t  *)calloc((size_t)_sieve_limit + 1, sizeof(uint8_t));

    /* linear sieve for spf */
    uint32_t *primes = (uint32_t *)malloc((size_t)_sieve_limit / 10 * sizeof(uint32_t));
    size_t   primes_cnt = 0;
    for (uint32_t i = 2; i <= _sieve_limit; ++i) {
        if (spf[i] == 0) {                     /* i is prime */
            spf[i] = i;
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t mul = (uint64_t)i * p;
            if (mul > _sieve_limit) break;
            spf[mul] = p;
            if (p == spf[i]) break;
        }
    }

    /* compute big Omega using stored spf */
    omega[1] = 0;
    for (uint32_t i = 2; i <= _sieve_limit; ++i) {
        omega[i] = omega[i / spf[i]] + 1;
    }

    /* collect all numbers with Omega == 5 */
    for (uint32_t i = 2; i <= _sieve_limit; ++i) {
        if (omega[i] == 5) ++terms_count;
    }
    terms = (seq_t *)malloc(terms_count * sizeof(seq_t));
    size_t idx = 0;
    for (uint32_t i = 2; i <= _sieve_limit; ++i) {
        if (omega[i] == 5) {
            terms[idx++] = (seq_t)i;
        }
    }

    free(primes);
}

/* -------------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* safety, index starts at 1 */

    init_sieve();

    /* If requested term beyond precomputed range, extend the sieve is not
       implemented – the problem guarantees n will fit in the precomputed
       table (at least up to n≈10,000). */
    if ((size_t)n > terms_count) return (seq_t)0;

    return terms[n - 1];
}
