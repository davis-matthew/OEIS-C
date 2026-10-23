/*
 * A023520  Exponent of 2 in prime factorization of prime(n)*prime(n-1) - 1.
 * https://oeis.org/A023520
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition:
   a(n) = exponent of 2 in prime(n)*prime(n-1) - 1
   (i.e. the largest k such that 2^k divides prime(n)*prime(n-1)-1)  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cache of first primes (stored as 64‑bit values). */
static uint64_t *prime_cache = NULL;   /* array of primes */
static size_t   prime_cache_cnt = 0;   /* number of primes stored */
static size_t   prime_cache_cap = 0;   /* current capacity of the array */

/* Ensure that prime_cache has at least n primes stored (1‑indexed). */
static void _ensure_prime_cache(idx_t n)
{
    if (prime_cache_cnt >= n)
        return;

    /* Grow capacity if necessary */
    if (prime_cache_cap == 0) {
        prime_cache_cap = 16;
        prime_cache = (uint64_t*)malloc(prime_cache_cap * sizeof(uint64_t));
    }
    while (prime_cache_cap < n)
        prime_cache_cap <<= 1;
    if (prime_cache_cap > prime_cache_cnt)
        prime_cache = (uint64_t*)realloc(prime_cache, prime_cache_cap * sizeof(uint64_t));

    /* Initial prime 2 if cache empty */
    if (prime_cache_cnt == 0) {
        prime_cache[0] = 2;
        prime_cache_cnt = 1;
    }

    /* Generate primes until we have n of them */
    uint64_t cand = (prime_cache_cnt == 1) ? 3 : prime_cache[prime_cache_cnt-1] + 2;
    while (prime_cache_cnt < n) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cache_cnt; ++i) {
            uint64_t p = prime_cache[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            prime_cache[prime_cache_cnt++] = cand;
        cand += 2;
    }
}

/* Exponent of 2 in (prime(n)*prime(n‑1) – 1) */
seq_t a(idx_t n)
{
    if (n < 2)                /* defined for n ≥ 2; return 0 for safety */
        return (seq_t)0;

    _ensure_prime_cache(n);   /* guarantees primes[n‑1] and primes[n‑2] exist */

    uint64_t p1 = prime_cache[n - 1]; /* prime(n)   */
    uint64_t p2 = prime_cache[n - 2]; /* prime(n-1) */

    seq_t val = ((seq_t)p1 * (seq_t)p2) - (seq_t)1;
    seq_t exp = 0;
    while ((val & (seq_t)1) == 0) {
        ++exp;
        val >>= 1;
    }
    return exp;
}
