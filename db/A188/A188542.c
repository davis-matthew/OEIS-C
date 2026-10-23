/*
 * A188542  Number of primes between n^3-n and n^3+n.
 * https://oeis.org/A188542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes p with n^3-n <= p <= n^3+n (inclusive)
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 423, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t startS = n3 - nn;
    seq_t endS   = n3 + nn;

    /* Convert to uint64_t for loop indices; safe because values fit in 64 bits */
    uint64_t start = (uint64_t)startS;
    uint64_t end   = (uint64_t)endS;
    uint64_t len   = end - start + 1;

    /* Allocate interval array; 1 means "assumed prime" */
    unsigned char *is_prime = (unsigned char*)malloc(len);
    if (!is_prime) return 0;                     /* allocation failed */
    memset(is_prime, 1, len);

    /* Mark 0 and 1 as non‑prime if they lie in the interval */
    if (start <= 1 && end >= 1) {
        if (1 >= start) is_prime[1 - start] = 0;
    }
    if (start == 0) {
        is_prime[0 - start] = 0;
    }

    /* Compute upper bound for the sieve: floor(sqrt(end)) */
    seq_t sqrtSeq = floor_sqrt(endS);
    uint64_t sieveLimit = (uint64_t)sqrtSeq + 1;

    /* Simple sieve of Eratosthenes up to sieveLimit to obtain primes */
    unsigned char *sieve = (unsigned char*)calloc(sieveLimit + 1, 1);
    if (!sieve) { free(is_prime); return 0; }
    for (uint64_t i = 2; i * i <= sieveLimit; ++i) {
        if (!sieve[i]) {
            for (uint64_t j = i * i; j <= sieveLimit; j += i)
                sieve[j] = 1;
        }
    }
    /* Collect primes */
    uint64_t *primes = NULL;
    size_t primesCap = 0, primesCnt = 0;
    for (uint64_t i = 2; i <= sieveLimit; ++i) {
        if (!sieve[i]) {
            if (primesCnt == primesCap) {
                primesCap = primesCap ? primesCap * 2 : 64;
                uint64_t *tmp = (uint64_t*)realloc(primes, primesCap * sizeof(uint64_t));
                if (!tmp) { free(is_prime); free(sieve); free(primes); return 0; }
                primes = tmp;
            }
            primes[primesCnt++] = i;
        }
    }
    free(sieve);

    /* Segmented sieve: mark composites in [start, end] */
    for (size_t pi = 0; pi < primesCnt; ++pi) {
        uint64_t p = primes[pi];
        uint64_t first = (start + p - 1) / p * p;
        for (uint64_t j = first; j <= end; j += p) {
            if (j == p) continue;          /* skip the prime itself */
            is_prime[j - start] = 0;
        }
    }
    free(primes);

    /* Count primes */
    uint64_t count = 0;
    for (uint64_t i = 0; i < len; ++i) {
        if (is_prime[i]) ++count;
    }
    free(is_prime);
    return (seq_t)count;
}
