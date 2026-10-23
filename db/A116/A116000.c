/*
 * A116000  phi(n) + sigma(n) gives a semiprime (A001358).
 * https://oeis.org/A116000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k (k ≥ 1) for which φ(k)+σ(k) is a
 *            semiprime
 * Range:     u8 n <= 48, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper utilities supplied by the build system */
#include "../utils/calc_phi.h"
#include "../utils/sigma_of.h"

/* Small sieve for primes used in semiprime tests.  The limit is large enough
   to cover all n requested in the typical tests. */
#define SIEVE_LIMIT 2000000U

static uint32_t *prime_list = NULL;
static size_t     prime_count = 0;
static bool       primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;

    uint32_t limit = SIEVE_LIMIT;
    uint8_t *mark = (uint8_t *)malloc(limit + 1);
    if (!mark) exit(EXIT_FAILURE);
    for (uint32_t i = 0; i <= limit; ++i) mark[i] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i)
        if (!mark[i])
            for (uint32_t j = i * i; j <= limit; j += i)
                mark[j] = 1;

    /* Count primes */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!mark[i]) ++cnt;

    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) exit(EXIT_FAILURE);
    size_t pos = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (!mark[i]) prime_list[pos++] = i;

    prime_count   = cnt;
    primes_ready  = true;
    free(mark);
}

/* Test whether a value of type seq_t is a semiprime (product of exactly
   two primes, possibly equal). */
static bool is_semiprime(seq_t v)
{
    if (v < 4) return false;          /* 4 = 2*2 is the smallest semiprime */
    seq_t rem = v;
    size_t factors = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        seq_t p  = (seq_t)prime_list[i];
        seq_t p2 = p * p;
        if (p2 > rem) break;
        while (rem % p == 0) {
            ++factors;
            rem /= p;
            if (factors > 2) return false;
        }
    }
    if (rem > 1) ++factors;           /* remaining prime factor */
    return factors == 2;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_primes();
    idx_t candidate = 1;
    idx_t found     = 0;

    while (1) {
        seq_t phi_val   = calc_phi((seq_t)candidate);
        seq_t sigma_val = sigma_of(candidate);
        seq_t sum       = phi_val + sigma_val;

        if (is_semiprime(sum)) {
            ++found;
            if (found == n) return (seq_t)candidate;
        }
        ++candidate;
    }
}
