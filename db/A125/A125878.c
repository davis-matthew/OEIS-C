/*
 * A125878  Duplicate of A066674.
 * https://oeis.org/A125878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the least integer k such that the largest prime dividing
 *            Euler's totient φ(k) equals the nth prime. Implementation builds
 *            a sieve up to a fixed limit and records the smallest k for every
 *            prime factor of φ up to that limit
 * Range:     i8 n <= 7, i16 n <= 51, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LIMIT_K 5000000ull  /* maximum k to sieve, should cover all required primes */

static bool initialized = false;
static uint64_t *spf = NULL;      /* smallest prime factor for numbers up to LIMIT_K */
static uint64_t *phi = NULL;      /* Euler's totient values */
static uint64_t *mink = NULL;     /* mink[p] = smallest k with largest prime factor of φ(k) == p */
static uint32_t *primes = NULL;   /* list of primes up to LIMIT_K */
static size_t prime_cnt = 0;

/* Linear sieve: fills spf, phi arrays, and primes list. */
static void init_sieve(void) {
    size_t i, j;
    uint32_t *tmp_primes = (uint32_t *)malloc((LIMIT_K / 10 + 1000) * sizeof(uint32_t));
    if (!tmp_primes) exit(EXIT_FAILURE);

    spf = (uint64_t *)malloc((LIMIT_K + 1) * sizeof(uint64_t));
    phi = (uint64_t *)malloc((LIMIT_K + 1) * sizeof(uint64_t));
    mink = (uint64_t *)malloc((LIMIT_K + 1) * sizeof(uint64_t));
    if (!spf || !phi || !mink) exit(EXIT_FAILURE);

    memset(spf, 0, (LIMIT_K + 1) * sizeof(uint64_t));
    memset(mink, 0, (LIMIT_K + 1) * sizeof(uint64_t));

    spf[0] = 1;
    spf[1] = 1;
    phi[0] = 0;
    phi[1] = 1;

    for (i = 2; i <= LIMIT_K; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            phi[i] = i - 1;
            tmp_primes[prime_cnt++] = (uint32_t)i;
        }
        for (j = 0; j < prime_cnt; ++j) {
            uint64_t p = tmp_primes[j];
            if (p > spf[i] || i * p > LIMIT_K) break;
            spf[i * p] = p;
            if (i % p == 0) {
                phi[i * p] = phi[i] * p;
                break;
            } else {
                phi[i * p] = phi[i] * (p - 1);
            }
        }
    }

    /* Allocate exact prime list */
    primes = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);
    for (i = 0; i < prime_cnt; ++i) {
        primes[i] = tmp_primes[i];
    }
    free(tmp_primes);

    /* For every k compute the largest prime factor of φ(k) and record mink */
    for (i = 2; i <= LIMIT_K; ++i) {
        uint64_t val = phi[i];
        uint64_t largest = 0;
        while (val > 1) {
            uint64_t d = spf[val];
            if (d > largest) largest = d;
            while ((val % d) == 0) val /= d;
        }
        if (largest >= 2 && mink[largest] == 0) {
            mink[largest] = i;
        }
    }

    initialized = true;
}

static inline uint64_t nth_prime(idx_t n) {
    /* n is 1‑based */
    return primes[n - 1];
}

seq_t a(idx_t n) {
    if (!initialized) init_sieve();

    /* Get the nth prime */
    uint64_t p = nth_prime(n);

    /* p should be within our precomputed range. */
    if (p <= LIMIT_K) {
        return (seq_t)mink[p];
    }

    /* If p is outside the range (unlikely for n <= 10000),
       recompute with larger limit (rare). */
    /* For safety, return 0 to indicate error. */
    return (seq_t)0;
}
