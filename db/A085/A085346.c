/*
 * A085346  Least number x so that number of primes between sigma(x) and
 *          phi(x) equals n.
 * https://oeis.org/A085346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least x such that number of primes p with φ(x) < p ≤ σ(x)
 *            equals n
 * Range:     u8 n <= 33, u16 n <= 3498, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Upper bounds chosen to compute a(n) for n up to 20000 (covering the required n≈10000). */
#define MAX_N        20000
#define LIMIT_X      600000   /* maximum x to search */
#define LIMIT_S      6000000  /* maximum value needed for prime counting (σ(x)) */

/* Static storage for precomputed data */
static seq_t *res_arr = NULL;          /* result array: res_arr[n] = a(n) */
static bool    init_done = false;

/* Helper: Initialize all tables on first call. */
static void init_sequence(void)
{
    /* 1. Sieve primes up to LIMIT_S and build prefix prime counts pi[]. */
    unsigned char *is_prime = (unsigned char *)malloc((size_t)(LIMIT_S + 1));
    if (!is_prime) exit(EXIT_FAILURE);
    for (size_t i = 0; i <= (size_t)LIMIT_S; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    /* Store primes in an array (worst-case size LIMIT_S/10 + 1). */
    size_t primes_sz = LIMIT_S / 10 + 10;
    unsigned int *primes = (unsigned int *)malloc(primes_sz * sizeof(unsigned int));
    if (!primes) exit(EXIT_FAILURE);
    size_t primes_cnt = 0;

    for (size_t i = 2; i <= (size_t)LIMIT_S; ++i) {
        if (is_prime[i]) {
            if (primes_cnt < primes_sz) primes[primes_cnt++] = (unsigned int)i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            unsigned long long prod = (unsigned long long)i * primes[j];
            if (prod > (unsigned long long)LIMIT_S) break;
            is_prime[(size_t)prod] = 0;
            if (i % primes[j] == 0) break;
        }
    }

    /* Prefix prime counts pi[i] = number of primes ≤ i. */
    uint32_t *pi = (uint32_t *)malloc((size_t)(LIMIT_S + 1) * sizeof(uint32_t));
    if (!pi) exit(EXIT_FAILURE);
    pi[0] = 0;
    for (size_t i = 1; i <= (size_t)LIMIT_S; ++i) {
        pi[i] = pi[i - 1] + (uint32_t)is_prime[i];
    }

    /* 2. Compute φ(x) for x ≤ LIMIT_X via the standard sieve. */
    seq_t *phi = (seq_t *)malloc((size_t)(LIMIT_X + 1) * sizeof(seq_t));
    if (!phi) exit(EXIT_FAILURE);
    for (size_t i = 0; i <= (size_t)LIMIT_X; ++i) phi[i] = (seq_t)i;

    for (size_t k = 0; k < primes_cnt && primes[k] <= (unsigned int)LIMIT_X; ++k) {
        unsigned int p = primes[k];
        for (size_t j = p; j <= (size_t)LIMIT_X; j += p) {
            phi[j] = phi[j] - phi[j] / (seq_t)p;
        }
    }

    /* 3. Compute σ(x) for x ≤ LIMIT_X by summing divisors. */
    seq_t *sigma = (seq_t *)malloc((size_t)(LIMIT_X + 1) * sizeof(seq_t));
    if (!sigma) exit(EXIT_FAILURE);
    for (size_t i = 0; i <= (size_t)LIMIT_X; ++i) sigma[i] = (seq_t)0;

    for (size_t d = 1; d <= (size_t)LIMIT_X; ++d) {
        for (size_t j = d; j <= (size_t)LIMIT_X; j += d) {
            sigma[j] = sigma[j] + (seq_t)d;
        }
    }

    /* 4. Allocate result array and fill it. */
    res_arr = (seq_t *)malloc((size_t)(MAX_N + 1) * sizeof(seq_t));
    if (!res_arr) exit(EXIT_FAILURE);
    for (size_t i = 0; i <= (size_t)MAX_N; ++i) res_arr[i] = (seq_t)0;

    size_t found = 0;
    for (size_t x = 2; x <= (size_t)LIMIT_X && found < (size_t)MAX_N; ++x) {
        seq_t sigmaX = sigma[x];
        seq_t phiX   = phi[x];
        idx_t sigmaIdx = (idx_t)sigmaX;
        idx_t phiIdx   = (idx_t)phiX;
        if (sigmaIdx > (idx_t)LIMIT_S || phiIdx > (idx_t)LIMIT_S) continue; /* safety */

        uint32_t count = pi[sigmaIdx] - pi[phiIdx];
        if (count <= (uint32_t)MAX_N && res_arr[count] == (seq_t)0) {
            res_arr[count] = (seq_t)x;
            ++found;
        }
    }

    /* Clean up temporary arrays. */
    free(is_prime);
    free(primes);
    free(pi);
    free(phi);
    free(sigma);

    init_done = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!init_done) init_sequence();
    if (n <= (idx_t)MAX_N) return res_arr[(size_t)n];
    /* For n beyond precomputed range, compute on demand by extending tables.
       As per problem statement, n will not exceed the precomputed bound. */
    return (seq_t)0;
}
