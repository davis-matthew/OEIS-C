/*
 * A059310  Primes p such that x^21 = 2 has no solution mod p.
 * https://oeis.org/A059310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes, gcd_uint64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A059310: primes p such that x^21 = 2 has no solution modulo p.
   The sequence is obtained by testing primes until we have found n such
   primes satisfying the condition. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/mod_pow.h"      /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/build_primes.h" /* void build_primes(idx_t limit, idx_t **out, size_t *cnt); */
#include "../utils/gcd_uint64.h"

static idx_t *primes        = NULL;   /* array of primes up to current sieve_limit */
static size_t prime_count   = 0;

static idx_t *sat_primes    = NULL;   /* primes that satisfy the condition */
static size_t sat_count     = 0;
static size_t sat_cap       = 0;

static idx_t sieve_limit    = 0;
static bool initialized     = false;

/* Rebuild the list of satisfying primes from the current primes array. */
static void rebuild_satisfying(void)
{
    if (primes == NULL)
        return;

    if (sat_primes) {
        free(sat_primes);
        sat_primes = NULL;
    }
    sat_cap   = 0;
    sat_count = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = primes[i];
        if (p < 7)          /* first term is 7 */
            continue;

        uint64_t g   = gcd_uint64(21ULL, (uint64_t)p - 1ULL);
        uint64_t exp = ((uint64_t)p - 1ULL) / g;

        seq_t r = mod_pow((seq_t)2, exp, (seq_t)p);
        if (r != (seq_t)1) {
            if (sat_count == sat_cap) {
                sat_cap = sat_cap ? sat_cap * 2 : 8;
                sat_primes = (idx_t *)realloc(sat_primes, sat_cap * sizeof(idx_t));
            }
            sat_primes[sat_count++] = p;
        }
    }
}

/* Ensure that we have found at least n satisfying primes. */
static void ensure_sat(idx_t n)
{
    if (sat_count >= n)
        return;

    if (sieve_limit < 10)
        sieve_limit = 10;

    while (sat_count < n) {
        sieve_limit *= 2;
        if (primes) {
            free(primes);
            primes = NULL;
        }
        build_primes(sieve_limit, &primes, &prime_count);
        rebuild_satisfying();
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = true;
        ensure_sat(1);
    }
    ensure_sat(n);
    return (seq_t)sat_primes[n - 1];
}
