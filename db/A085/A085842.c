/*
 * A085842  Numbers k whose divisors (apart from 1 and k) sum to a prime.
 * https://oeis.org/A085842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k whose divisors (excluding 1 and k) sum
 *            to a prime
 * Range:     u8 n <= 36, u16 n <= 5635, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static storage for the computed sequence */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Current number that we are checking */
static uint64_t current_k = 1;

/* Primes up to a fixed limit (sufficient for k up to ~10^12) */
#define PRIME_MAX 1000000
static uint32_t prime_list[78498];
static size_t prime_count = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited)
        return;
    primes_inited = true;
    bool *is_prime = (bool *)malloc(PRIME_MAX + 1);
    for (size_t i = 0; i <= PRIME_MAX; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= PRIME_MAX; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= PRIME_MAX; j += i)
                is_prime[j] = false;
    for (size_t i = 2; i <= PRIME_MAX; ++i)
        if (is_prime[i])
            prime_list[prime_count++] = (uint32_t)i;
    free(is_prime);
}

/* Simple primality test for 64‑bit integers */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2)
        return false;
    if (x % 2 == 0)
        return x == 2;
    if (x % 3 == 0)
        return x == 3;
    for (uint64_t i = 5; i * i <= x; i += 6) {
        if (x % i == 0 || x % (i + 2) == 0)
            return false;
    }
    return true;
}

/* Sum of all divisors of n (including 1 and n) */
static uint64_t sigma_uint64(uint64_t n)
{
    uint64_t res = 1;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint32_t p = prime_list[i];
        if (tmp % p == 0) {
            uint64_t term = 1;
            uint64_t power = 1;
            while (tmp % p == 0) {
                tmp /= p;
                power *= (uint64_t)p;
                term += power;
            }
            res *= term;
        }
    }
    if (tmp > 1) /* remaining prime factor */
        res *= (1 + tmp);
    return res;
}

/* Check whether a number k satisfies the OEIS condition */
static bool satisfies(uint64_t k)
{
    if (k < 2)
        return false;
    uint64_t sigma = sigma_uint64(k);
    if (sigma <= k + 1)   /* sum would be 0 or negative */
        return false;
    uint64_t div_sum = sigma - 1 - k; /* sum of divisors except 1 and k */
    return is_prime_uint64(div_sum);
}

/* Append a new term to the cache */
static void push_cache(seq_t v)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
    cache[cache_len++] = v;
}

/* Expand the cache until it contains at least n terms */
static void ensure_cache(idx_t n)
{
    init_primes();          /* be sure primes are ready */
    while (cache_len < (size_t)n) {
        ++current_k;
        if (satisfies(current_k))
            push_cache((seq_t)current_k);
    }
}

/* Public accessor for a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for offset 1, return 0 as safe */
    ensure_cache(n);
    return cache[n - 1];
}
