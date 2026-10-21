/*
 * A215351  Primes congruent to {2, 3, 4, 6} mod 13.
 * https://oeis.org/A215351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p for which p mod 13 ∈ {2,3,4,6}
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Caching of found primes */
static seq_t *prime_cache = NULL;   /* array of cached primes */
static size_t prime_len   = 0;       /* number of primes stored */
static size_t prime_cap   = 0;       /* current allocation capacity */
static uint64_t next_candidate = 2;  /* next integer to test for primality */

/* Simple deterministic primality test for 64‑bit integers */
static bool isPrime64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint64_t i = 5;
    /* loop up to sqrt(n), testing i and i+2 */
    while (i <= n / i) {
        if (n % i == 0) return false;
        if (n % (i + 2) == 0) return false;
        i += 6;
    }
    return true;
}

/* Ensure that prime_cache contains at least 'need' primes. */
static void build_cache(idx_t need) {
    if (prime_len >= (size_t)need) return;          /* already enough */

    /* Grow the array if necessary */
    if (prime_cap < (size_t)need) {
        size_t new_cap = prime_cap == 0 ? 16 : prime_cap * 2;
        while (new_cap < (size_t)need) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                          /* allocation failed */
        prime_cache = tmp;
        prime_cap = new_cap;
    }

    /* Generate primes until the requested number is reached */
    while (prime_len < (size_t)need) {
        uint64_t r = next_candidate % 13;
        if ((r == 2 || r == 3 || r == 4 || r == 6) && isPrime64(next_candidate)) {
            prime_cache[prime_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* Public function: a(n) */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* below offset; value is irrelevant */
    build_cache(n);
    return prime_cache[n - 1];
}
