/*
 * A106350  Semiprimes indexed by primes.
 * https://oeis.org/A106350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = semiprime(prime(n))
 * Range:     u8 none, u16 n <= 28, u32 n <= 5094, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 8898 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth semiprime indexed by the nth prime.
   Caches primes and semiprime list across calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/mem.h"

/* -------------------------------------------------------------------- */
static uint64_t estimate_prime_upper_bound(idx_t n)
{
    if (n < 6) return 15;  /* covers first 6 primes                        */
    double nn    = (double)n;
    double bound = nn * (log(nn) + log(log(nn)));
    return (uint64_t)(bound + 3.0);  /* safety margin                         */
}

/* Helper: compare two seq_t values for qsort */
static int cmp_seq(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return  1;
    return 0;
}

/* Global state for primes and semiprimes */
static seq_t *primes          = NULL;
static size_t  primeCnt       = 0;
static uint64_t primeLimit    = 0;          /* highest number sieved for primes */

static seq_t *semiprimes      = NULL;
static size_t  semiprimeCnt   = 0;
static uint64_t semiprimeLimit = 0;        /* highest number generated for semiprimes */

/* Sieve all primes up to 'limit' (inclusive) */
static void sieve_primes(uint64_t limit)
{
    if (limit <= primeLimit) return;

    char *isComposite = (char *)calloc(limit + 1, sizeof(char));
    if (!isComposite) exit(1);

    for (uint64_t i = 2; i * i <= limit; ++i)
        if (!isComposite[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                isComposite[j] = 1;

    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!isComposite[i]) ++cnt;

    seq_t *newPrimes = mem_resize(seq_t, primes, cnt);
    if (!newPrimes) exit(1);
    primes = newPrimes;
    primeCnt = cnt;

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (!isComposite[i]) primes[idx++] = (seq_t)i;

    free(isComposite);
    primeLimit = limit;
}

/* Generate all semiprimes <= 'limit' and store in sorted array */
static void generate_semiprimes(uint64_t limit)
{
    /* Ensure primes up to 'limit' are available (needed for p*q <= limit) */
    sieve_primes(limit);

    /* Estimate capacity: fewer than 'limit' semiprimes will exist */
    size_t estCap = (size_t)(limit / 2 + 10);
    seq_t *buf = mem_new(seq_t, estCap);
    if (!buf) exit(1);
    size_t count = 0;

    for (size_t i = 0; i < primeCnt && primes[i] <= limit; ++i) {
        seq_t p = primes[i];
        for (size_t j = i; j < primeCnt && primes[j] <= limit; ++j) {
            seq_t q = primes[j];
            seq_t prod = (seq_t)p * (seq_t)q;
            if (prod > limit) break;
            if (count == estCap) {
                estCap <<= 1;
                buf = mem_resize(seq_t, buf, estCap);
                if (!buf) exit(1);
            }
            buf[count++] = prod;
        }
    }

    mem_qsort(buf, count, sizeof(seq_t), cmp_seq);

    mem_free(semiprimes);
    semiprimes = buf;
    semiprimeCnt = count;
    semiprimeLimit = limit;
}

/* Main function: a(n) = semiprime(prime(n)) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for position 0 */

    /* Ensure we have at least 'n' primes */
    if (primeCnt < (size_t)n) {
        uint64_t limit = estimate_prime_upper_bound((idx_t)n) + 1000;
        sieve_primes(limit);
    }

    seq_t nthPrime = primes[(size_t)n - 1];        /* prime(n) */

    /* Ensure we have enough semiprimes to access index nthPrime */
    if (semiprimeCnt < (size_t)nthPrime) {
        /* Start with a modest limit and double until we have enough */
        uint64_t limit = (semiprimeLimit > 0) ? semiprimeLimit * 2 : 1000;
        do {
            generate_semiprimes(limit);
            if (semiprimeCnt >= (size_t)nthPrime) break;
            limit *= 2;
        } while (true);
    }

    return semiprimes[(size_t)nthPrime - 1];
}
