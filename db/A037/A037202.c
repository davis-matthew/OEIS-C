/*
 * A037202  Number of lines in Pratt certificate for n-th prime.
 * https://oeis.org/A037202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1 + Sum a(p), p prime, p | (prime_n - 1) (a(2)=1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, build_spf, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n) where n is the index (1‑based) of the prime. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/build_primes_upto.h"
#include "../utils/build_spf.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_alloc_new_array.h"

/* static state */
static uint64_t *primes = NULL;            /* list of primes up to current limit */
static size_t prime_cnt = 0;               /* number of primes available       */
static uint64_t prime_limit = 0;           /* current sieve limit              */

static idx_t *spf = NULL;                  /* smallest prime factor array      */
static uint64_t spf_limit = 0;             /* size of spf array                */

static seq_t *cert_by_value = NULL;        /* cert_by_value[p] -> a(p)         */
static uint64_t cert_by_value_limit = 0;   /* size of cert_by_value array      */

static seq_t *cert = NULL;                 /* certificate array by index      */
static size_t cert_cap = 0;                /* capacity of cert                */
static size_t cert_len = 0;                /* number of certificates computed */

/* Ensure that primes up to index n are available. */
static void ensure_primes_and_spf(idx_t n)
{
    if (primes && n <= prime_cnt)   /* already have enough primes */
        return;

    /* Rough upper bound for the nth prime: n * 20 + 10   (safe for n up to 1e6) */
    uint64_t limit;
    if (n < 6)
        limit = 15;
    else
        limit = n * 20 + 10;

    if (limit < prime_limit)
        limit = prime_limit;      /* keep previous limit if larger */

    uint64_t *new_primes = NULL;
    size_t new_cnt = 0;
    build_primes_upto(limit, &new_primes, &new_cnt);

    /* Replace old array */
    free(primes);
    primes = new_primes;
    prime_cnt = new_cnt;
    prime_limit = limit;

    /* Rebuild spf array for new limit */
    if (spf)
        free(spf);
    build_spf(limit, (idx_t**)&spf);
    spf_limit = limit;

    /* Resize cert_by_value array if needed */
    if (cert_by_value_limit < (prime_limit + 1)) {
        seq_t *tmp = seq_realloc(cert_by_value, cert_by_value_limit, prime_limit + 1);
        memset(tmp + cert_by_value_limit, 0,
               (prime_limit + 1 - cert_by_value_limit) * sizeof(seq_t));
        cert_by_value = tmp;
        cert_by_value_limit = prime_limit + 1;
    }
}

/* Compute certificates up to index n */
static void ensure_certificates(idx_t n)
{
    ensure_primes_and_spf(n);

    if (cert_len >= (size_t)n)
        return;   /* already computed */

    /* Resize cert array if needed */
    if (cert_cap < (size_t)n) {
        seq_t *tmp = seq_realloc(cert, cert_cap, n);
        memset(tmp + cert_cap, 0, (n - cert_cap) * sizeof(seq_t));
        cert = tmp;
        cert_cap = n;
    }

    /* Compute certificates for indices cert_len .. n-1 */
    for (size_t idx = cert_len; idx < (size_t)n; ++idx) {
        uint64_t p = primes[idx];          /* the prime at this index (0‑based)   */
        seq_t sum = 1;                     /* the "+1" from the recurrence       */

        uint64_t m = p - 1;
        while (m > 1) {
            uint64_t d = spf[m];           /* smallest prime factor of m         */
            sum += cert_by_value[d];       /* add certificate of that prime      */
            while (m % d == 0)
                m /= d;                    /* skip multiple occurrences         */
        }

        cert[idx] = sum;
        cert_by_value[p] = sum;            /* record for future use              */
    }

    cert_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, but guard anyway */
        return 0;

    ensure_certificates(n);
    return cert[n - 1];   /* sequence is 1‑based */
}
