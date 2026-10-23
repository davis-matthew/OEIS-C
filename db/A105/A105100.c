/*
 * A105100  Sum of ordered 3 prime sided prime triangles.
 * https://oeis.org/A105100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0}^{5} p(6n-5+k) (p is the k‑th prime)
 * Range:     u8 n <= 2, u16 n <= 221, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Prime generation and caching */
static uint64_t *prime_cache = NULL;   /* array of primes */
static idx_t prime_count   = 0;        /* number of primes stored */
static uint64_t next_candidate = 2;    /* next odd number to test */

/* Ensure we have at least 'required' primes cached */
static void ensure_primes(idx_t required)
{
    if (prime_count >= required)
        return;

    /* grow the cache if necessary */
    idx_t new_cap = prime_count ? prime_count : 128;
    while (new_cap < required)
        new_cap <<= 1;
    if (prime_cache == NULL) {
        prime_cache = (uint64_t *)malloc(new_cap * sizeof(uint64_t));
    } else {
        prime_cache = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    }

    /* generate primes up to the required count */
    while (prime_count < required) {
        uint64_t cand = next_candidate;
        int is_prime = 1;
        for (idx_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_cache[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            prime_cache[prime_count++] = cand;
        }
        ++next_candidate;
    }
}

/* A105100: sum of the six consecutive primes that form a prime triangle */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* offset is 1, so n<1 is irrelevant */
    idx_t needed = n * (idx_t)6;             /* we need primes up to index 6n */
    ensure_primes(needed);

    idx_t start = needed - 5;                /* index of the first prime in this block (1‑based) */
    idx_t idx   = start - 1;                 /* zero‑based array index */

    uint64_t sum = 0;
    for (idx_t i = idx; i < idx + 6; ++i)
        sum += prime_cache[i];

    return (seq_t)sum;
}
