/*
 * A145539  Number of numbers removed in each step of Eratosthenes's sieve for
 *          10^6.
 * https://oeis.org/A145539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 168, u64 n <= 168, u128 n <= 168,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via a sieve:  a(n) = number of composite integers ≤ 10⁶ whose
   least prime factor equals the n‑th prime (primes indexed from 1). */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

#define LIMIT (idx_t)1000000ULL

static seq_t *counts = NULL;          /* counts per prime, 0‑based */
static size_t num_primes = 0;         /* number of primes ≤ LIMIT */
static size_t *prime_to_index = NULL; /* maps prime value → 1‑based prime index */
static bool initialized = false;

static void compute_once(void)
{
    if (initialized) return;

    /* 1. Get all primes ≤ LIMIT */
    uint64_t *primes = NULL;
    size_t cnt = 0;
    build_primes_upto(LIMIT, &primes, &cnt);
    num_primes = cnt;

    /* 2. Allocate lookup tables */
    size_t limit_sz = (size_t)LIMIT + 1;
    prime_to_index = (size_t *)calloc(limit_sz, sizeof(size_t));
    counts = (seq_t *)calloc((size_t)num_primes, sizeof(seq_t));

    /* 3. Map each prime to its 1‑based index (1‑based for easier use later) */
    for (size_t i = 0; i < num_primes; ++i) {
        uint64_t p = primes[i];
        prime_to_index[p] = i + 1;
    }

    /* 4. Compute smallest prime factor (spf) for every number up to LIMIT */
    uint32_t *spf = (uint32_t *)calloc(limit_sz, sizeof(uint32_t));
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {               /* i is prime */
            spf[i] = (uint32_t)i;
            if ((idx_t)i * i <= LIMIT) {
                for (idx_t j = i * i; j <= LIMIT; j += i) {
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }

    /* 5. Count composite numbers by their least prime factor */
    for (idx_t m = 2; m <= LIMIT; ++m) {
        uint32_t p = spf[m];
        if (p != (uint32_t)m) {           /* composite number */
            size_t idx = prime_to_index[p] - 1; /* 0‑based index */
            counts[idx] += 1;
        }
    }

    /* 6. Free temporary tables */
    free(primes);
    free(spf);

    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, undefined for 0 */
    compute_once();
    if (n > (idx_t)num_primes) return 0; /* out of range → 0 */
    return counts[n - 1];
}
