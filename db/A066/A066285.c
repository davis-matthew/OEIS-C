/*
 * A066285  a(n) is the minimal difference between primes p and q whose sum is
 *          2n.
 * https://oeis.org/A066285
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = minimal difference between primes p and q with p + q = 2n
 *            (a(n) = 0 if n is prime)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* Helper structure to store a sieve of Eratosthenes */
typedef struct {
    uint8_t *isprime;   /* 0 = composite, 1 = prime */
    uint32_t *primes;   /* list of prime numbers (optional) */
    uint64_t max;       /* maximum number sieved (inclusive) */
    size_t prime_cnt;   /* number of primes stored */
} Sieve;

static Sieve sieve = {NULL, NULL, 0, 0};

/* Expand the sieve to cover numbers up to limit (inclusive). */
static void ensure_sieve(uint64_t limit)
{
    if (limit <= sieve.max) return;            /* already sufficient */
    uint64_t new_max = limit;
    uint8_t *arr = (uint8_t*)calloc((size_t)(new_max + 1), sizeof(uint8_t));
    if (!arr) exit(EXIT_FAILURE);
    /* 0 and 1 are not prime */
    if (new_max >= 0) arr[0] = 0;
    if (new_max >= 1) arr[1] = 0;
    for (uint64_t i = 2; i <= new_max; ++i) arr[i] = 1;
    uint64_t sq = (uint64_t)sqrt((double)new_max);
    for (uint64_t i = 2; i <= sq; ++i) {
        if (arr[i]) {
            for (uint64_t j = i * i; j <= new_max; j += i) arr[j] = 0;
        }
    }
    /* Build prime list */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= new_max; ++i)
        if (arr[i]) ++cnt;
    uint32_t *pl = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    if (!pl) exit(EXIT_FAILURE);
    size_t idx = 0;
    for (uint64_t i = 2; i <= new_max; ++i)
        if (arr[i]) pl[idx++] = (uint32_t)i;
    /* Free old data */
    free(sieve.isprime);
    free(sieve.primes);
    sieve.isprime = arr;
    sieve.primes = pl;
    sieve.max = new_max;
    sieve.prime_cnt = cnt;
}

/* Main function requested by the evaluator */
seq_t a(idx_t n)
{
    /* Ensure we have primes up to 2*n */
    uint64_t target = (uint64_t)n * 2;
    if ((uint64_t)n > 0) target = (uint64_t)n * 2;
    if (sieve.max < target) ensure_sieve(target);

    /* Search for the largest prime p <= n such that 2n-p is prime */
    for (uint64_t p = (uint64_t)n; p >= 2; --p) {
        if (sieve.isprime[p] && sieve.isprime[target - p]) {
            seq_t diff = (seq_t)2 * (seq_t)(n - p);
            return diff;
        }
        if (p == 2) break;  /* avoid underflow in unsigned loop */
    }
    /* If no partition found (should not happen for n >= 2), return 0 */
    return 0;
}
