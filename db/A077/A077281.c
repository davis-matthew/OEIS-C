/*
 * A077281  Square root of sum of successive primes in n-th group in A077280.
 * https://oeis.org/A077281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sqrt( sum of successive primes in the n‑th group whose
 *            total is a perfect square )
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----- helper: integer square root of 128‑bit value ----- */
static uint64_t isqrt128(unsigned __int128 x)
{
    unsigned __int128 r   = 0;
    unsigned __int128 bit = ((unsigned __int128)1) << 126;   /* highest power of 4 <= x */
    while (bit) {
        unsigned __int128 tmp = r + bit;
        if (tmp <= x / tmp)
            r = tmp;
        bit >>= 1;
    }
    return (uint64_t)r;
}

/* ----- helper: integer square root of 64‑bit value ----- */
static uint64_t isqrt64(uint64_t x)
{
    uint64_t r   = 0;
    uint64_t bit = 1ULL << 62;   /* highest power of 4 <= x */
    while (bit) {
        uint64_t tmp = r + bit;
        if (tmp <= x / tmp)
            r = tmp;
        bit >>= 1;
    }
    return r;
}

/* ----- prime sieve on the fly (trial division) ----- */
static uint64_t *prime_list   = NULL;
static size_t prime_count     = 0;
static size_t prime_capacity  = 0;
static uint64_t next_candidate = 2;  /* next number to test for primality */

/* ensure prime_list has enough capacity */
static void ensure_prime_capacity(size_t need)
{
    if (prime_capacity < need) {
        size_t new_cap = prime_capacity ? prime_capacity * 2 : 16;
        if (new_cap < need) new_cap = need;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        prime_list = tmp;
        prime_capacity = new_cap;
    }
}

/* test whether n is prime using the primes already found */
static bool isPrime(uint64_t n)
{
    if (n < 2) return false;
    uint64_t limit = isqrt64(n);
    for (size_t i = 0; i < prime_count && prime_list[i] <= limit; ++i)
        if (n % prime_list[i] == 0)
            return false;
    return true;
}

/* generate and return the next prime after the previous one */
static uint64_t next_prime(void)
{
    while (true) {
        if (isPrime(next_candidate)) {
            ensure_prime_capacity(prime_count + 1);
            prime_list[prime_count++] = next_candidate;
            uint64_t prime = next_candidate;
            ++next_candidate;   /* start next search from after this prime */
            return prime;
        }
        ++next_candidate;
    }
}

/* ----- main sequence implementation ----- */
seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* lazy initialization of the cache */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    /* generate groups until we have at least n terms */
    while (cache_len < n) {
        unsigned __int128 sum = 0;
        for (;;) {
            uint64_t p = next_prime();          /* prime > previous one */
            sum += (unsigned __int128)p;
            uint64_t r = isqrt128(sum);
            if ((unsigned __int128)r * r == sum) {
                /* store result */
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    cache = tmp;
                }
                cache[cache_len++] = (seq_t)r;
                break;
            }
            /* otherwise keep accumulating primes */
        }
    }

    return cache[n - 1];   /* 1‑based indexing */
}
