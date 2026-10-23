/*
 * A165915  a(n) = number of squarefree nonzero quadratic residues of n.
 * https://oeis.org/A165915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of squarefree non‑zero quadratic residues modulo n
 * Range:     u8 n <= 858, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Simple prime sieve up to limit; the caller must free the returned array. */
static uint32_t *sieve_primes(uint64_t limit, size_t *cnt)
{
    if (limit < 2) {
        *cnt = 0;
        return NULL;
    }
    uint8_t *is_prime = (uint8_t *)malloc(limit + 1);
    if (!is_prime) return NULL;
    for (uint64_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }
    /* count primes */
    *cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) if (is_prime[i]) ++(*cnt);
    /* allocate array of primes */
    uint32_t *primes = (uint32_t *)malloc(*cnt * sizeof(uint32_t));
    if (!primes) {
        free(is_prime);
        return NULL;
    }
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i) if (is_prime[i]) primes[idx++] = (uint32_t)i;
    free(is_prime);
    return primes;
}

/* Check if an integer x <= 2^63-1 is squarefree using the provided primes. */
static int is_squarefree(uint64_t x, const uint32_t *primes, size_t prime_cnt)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > x) break;
        if (x % (p * p) == 0) return 0;   /* not squarefree */
    }
    return 1;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;          /* a(1)=0, index offset 1 */

    size_t N = (size_t)n;
    uint8_t *residue = (uint8_t *)calloc(N, sizeof(uint8_t));
    if (!residue) return (seq_t)0;         /* allocation failure, fallback */

    /* Mark all quadratic residues modulo n (non‑zero included) */
    for (uint64_t x = 0; x < n; ++x) {
        uint64_t r = (x * x) % n;
        residue[r] = 1;
    }

    /* Generate primes up to sqrt(n) for squarefree tests */
    size_t prime_cnt;
    uint64_t limit = (uint64_t)floor(sqrt((double)n)) + 1;
    uint32_t *primes = sieve_primes(limit, &prime_cnt);
    if (!primes && limit >= 2) {
        free(residue);
        return (seq_t)0;                   /* sieve failure */
    }

    seq_t count = 0;
    for (uint64_t r = 1; r < n; ++r) {
        if (!residue[r]) continue;
        if (is_squarefree(r, primes, prime_cnt)) ++count;
    }

    free(residue);
    free(primes);
    return count;
}
