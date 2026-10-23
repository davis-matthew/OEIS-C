/*
 * A106350  Semiprimes indexed by primes.
 * https://oeis.org/A106350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = semiprime(prime(n))
 * Range:     u8 none, u16 n <= 28, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth semiprime indexed by the nth prime.
   Caches primes and semiprime list across calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/estimate_prime_upper_bound.h"

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

    seq_t *newPrimes = (seq_t *)realloc(primes, cnt * sizeof(seq_t));
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
    seq_t *buf = (seq_t *)malloc(estCap * sizeof(seq_t));
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
                buf = (seq_t *)realloc(buf, estCap * sizeof(seq_t));
                if (!buf) exit(1);
            }
            buf[count++] = prod;
        }
    }

    qsort(buf, count, sizeof(seq_t), cmp_seq);

    free(semiprimes);
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
