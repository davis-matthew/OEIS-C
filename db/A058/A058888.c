/*
 * A058888  Number of terms in the set invphi(2*prime(n)), where prime(n) is
 *          the n-th prime.
 * https://oeis.org/A058888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_phi, simple_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = number of solutions of φ(x)=2*prime(n) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/compute_phi.h"
#include "../utils/simple_sieve.h"

/* idx_t and seq_t are provided by the build environment */
static size_t B = 0;           /* maximum n for which φ(n) is computed */
static uint32_t *phi = NULL;   /* phi array */
static uint32_t *prime_list = NULL;  /* list of first primes */
static size_t prime_count = 0; /* number of primes stored */
static uint32_t *cnt_phi = NULL; /* count of solutions for each m */
static uint32_t maxM = 0;      /* maximum m = 2*prime[n] that will be queried */
static bool initialized = false;

static void init(void)
{
    if (initialized) return;
    /* Ensure we can compute up to n=10000 (10kth prime ~104729) */
    const size_t TARGET_N = 10000;
    /* First generate primes up to 110 000 to be safe */
    const uint32_t PRIMES_LIMIT = 110000;
    prime_list = NULL;
    prime_count = simple_sieve(PRIMES_LIMIT, &prime_list);
    if (prime_count < TARGET_N)
        return; /* defensive, but should not happen */

    /* Compute maximum m needed */
    uint32_t p_last = prime_list[TARGET_N - 1];
    maxM = 2 * p_last;

    /* Set B to be big enough to capture all n with φ(n) <= maxM
       Use B = 3*maxM + 100 for safety */
    B = 3 * maxM + 100;
    phi = (uint32_t *)malloc((B + 1) * sizeof(uint32_t));
    compute_phi(B, phi);

    /* Allocate cnt_phi array */
    cnt_phi = (uint32_t *)calloc(maxM + 1, sizeof(uint32_t));
    for (uint32_t n = 2; n <= B; ++n) {
        uint32_t m = phi[n];
        if (m <= maxM)
            cnt_phi[m]++;
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    init();
    if (n == 0 || (size_t)n > prime_count)
        return (seq_t)0; /* out of precomputed range, return 0 */
    uint32_t p = prime_list[n - 1];
    uint32_t m = 2 * p;
    return (seq_t)cnt_phi[m];
}
