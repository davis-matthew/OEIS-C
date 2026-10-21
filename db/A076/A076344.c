/*
 * A076344  A076341(A001358(n)), imaginary part of semiprimes mapped as
 *          defined in A076340, A076341.
 * https://oeis.org/A076344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   prime_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A076344: a(n) = imaginary part of the
   mapping defined in A076340 for the nth semiprime A001358(n). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/prime_pair.h"

/* Global tables for sieve */
static uint32_t *spf = NULL;        /* smallest prime factor for numbers ≤ limit */
static uint32_t *primes = NULL;     /* list of primes */
static size_t primes_len  = 0;
static size_t primes_cap  = 0;

/* Global array of semiprimes (in increasing order). */
static idx_t *semiprimes = NULL;
static size_t semiprimes_len = 0;
static size_t semiprimes_cap = 0;

/* Build a sieve of Eratosthenes with smallest prime factor array up to 'limit'. */
static void build_sieve(uint32_t limit)
{
    spf = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    if (!spf) return;                  /* allocation failed */

    /* dynamic prime list */
    primes_cap = limit / 10 + 1;
    primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) return;
    primes_len = 0;

    for (uint32_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            if (primes_len == primes_cap) {
                primes_cap = primes_cap * 2 + 1;
                primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_len++] = i;
        }
        for (size_t j = 0; j < primes_len; ++j) {
            uint32_t p = primes[j];
            if (p > spf[i] || (uint64_t)i * p > limit) break;
            spf[i * p] = p;
        }
    }

    /* Fill semiprimes up to 'limit'. */
    semiprimes_cap = 10000;
    semiprimes = (idx_t*)malloc(semiprimes_cap * sizeof(idx_t));
    if (!semiprimes) return;
    semiprimes_len = 0;

    for (uint32_t n = 4; n <= limit; ++n) {
        uint32_t cnt = 0;
        uint32_t m = n;
        while (m > 1 && cnt < 3) {
            ++cnt;
            m /= spf[m];
        }
        if (cnt == 2) {
            if (semiprimes_len == semiprimes_cap) {
                semiprimes_cap = semiprimes_cap * 2 + 1;
                semiprimes = (idx_t*)realloc(semiprimes, semiprimes_cap * sizeof(idx_t));
            }
            semiprimes[semiprimes_len++] = (idx_t)n;
        }
    }
}

/* Ensure that at least 'need' semiprimes are available. */
static void ensure_semiprimes(idx_t need)
{
    static uint32_t current_limit = 0;
    if (semiprimes_len >= (size_t)need) return;

    while (semiprimes_len < (size_t)need) {
        current_limit = current_limit ? current_limit * 2 : 10000;
        /* free previous tables */
        free(spf);  free(primes);  free(semiprimes);
        build_sieve(current_limit);
    }
}

/* Main function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */
    ensure_semiprimes(n);

    uint32_t m = (uint32_t)semiprimes[(size_t)n - 1];
    uint32_t p = spf[m];
    uint32_t q = m / p;

    seq_t ar, ai, br, bi;
    prime_pair(p, &ar, &ai);
    prime_pair(q, &br, &bi);

    /* Imaginary part of (ar + ai*i) * (br + bi*i) */
    seq_t imag_part = ar * bi + ai * br;
    return imag_part;
}
