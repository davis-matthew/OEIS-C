/*
 * A106588  Difference between n-th prime squared and n-th perfect square.
 * https://oeis.org/A106588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n)^2 - n^2
 * Range:     u8 n <= 7, u16 n <= 55, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cached list of primes found so far. */
static uint64_t *prime_list = NULL;
static size_t prime_len  = 0;
static size_t prime_cap  = 0;
static uint64_t next_candidate = 2;

/* Append a prime to the cache, growing the buffer if necessary. */
static void add_prime(uint64_t p)
{
    if (prime_len >= prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 64;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) abort();             /* out of memory */
        prime_list = tmp;
        prime_cap  = new_cap;
    }
    prime_list[prime_len++] = p;
}

/* Test whether x is prime using the cached primes. */
static bool is_prime_candidate(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;          /* p > sqrt(x) */
        if (x % p == 0) return false;
    }
    return true;
}

/* Return the n-th prime (n >= 1). */
static uint64_t nth_prime(idx_t n)
{
    while (prime_len < (size_t)n) {
        uint64_t cand = next_candidate;
        if (is_prime_candidate(cand))
            add_prime(cand);
        if (cand == 2)
            next_candidate = 3;
        else
            next_candidate += 2;         /* skip even numbers */
    }
    return prime_list[n - 1];
}

/* Public sequence function */
seq_t a(idx_t n)
{
    seq_t p  = (seq_t)nth_prime(n);
    seq_t ns = (seq_t)n;
    return p * p - ns * ns;
}
