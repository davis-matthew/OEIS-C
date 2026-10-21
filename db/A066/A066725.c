/*
 * A066725  Primes whose product of digits + 1 is also prime.
 * https://oeis.org/A066725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the n-th prime p such that p is prime and
 * product of its decimal digits + 1 is also prime.
 * This is computed by scanning primes in increasing order and
 * checking the digit‑product condition.  Results are cached so
 * subsequent calls are efficient.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Internal helper: primality test by trial division using a prime list */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    static uint32_t *prime_list = NULL;
    static size_t prime_count = 0, prime_cap = 0;
    /* Extend the prime list if needed */
    if (prime_count == 0) {
        prime_cap = 64;
        prime_list = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
        if (!prime_list) exit(EXIT_FAILURE);
        prime_list[prime_count++] = 2;
        prime_list[prime_count++] = 3;
    }
    /* Ensure prime list covers sqrt(n) */
    uint64_t limit = (uint64_t) ( (double) n ) + 1; /* rough upper bound */
    while ((uint64_t)prime_list[prime_count-1] * prime_list[prime_count-1] <= n) {
        uint32_t next_candidate = prime_list[prime_count-1] + 2;
        while (true) {
            bool square = false;
            for (size_t i = 0; i < prime_count; ++i) {
                if ((uint64_t)prime_list[i]*prime_list[i] > next_candidate) break;
                if (next_candidate % prime_list[i] == 0) { square = true; break; }
            }
            if (!square) break;
            next_candidate += 2;
        }
        if (prime_count == prime_cap) {
            prime_cap *= 2;
            prime_list = (uint32_t*)realloc(prime_list, prime_cap * sizeof(uint32_t));
            if (!prime_list) exit(EXIT_FAILURE);
        }
        prime_list[prime_count++] = next_candidate;
    }
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0, cache_cap = 0;
    static uint64_t last_checked = 1; /* last number tested for primality */
    if (n <= 0) return (seq_t)0; /* as per constraints n>=1, defensive */

    /* extend cache until we have at least n terms */
    while (cache_size < n) {
        /* find next prime */
        uint64_t p = last_checked + 1;
        while (!is_prime_uint64(p)) ++p;
        last_checked = p;

        /* compute product of decimal digits */
        uint64_t prod = 1;
        uint64_t tmp = p;
        while (tmp > 0) {
            prod *= (tmp % 10);
            tmp /= 10;
        }
        if (is_prime_uint64(prod + 1)) {
            /* new term found */
            if (cache_size == cache_cap) {
                cache_cap = (cache_cap == 0) ? 32 : cache_cap * 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) exit(EXIT_FAILURE);
            }
            cache[cache_size++] = (seq_t)p;
        }
    }
    return cache[n-1];
}
