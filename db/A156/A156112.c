/*
 * A156112  Primes p such that p+36 and p+72 are both prime.
 * https://oeis.org/A156112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 402, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: return the n‑th prime p for which p+36 and p+72 are also prime. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* push a value into a dynamic uint64_t array, reallocating if needed */
static void push_uint64(uint64_t **arr, size_t *cnt, size_t *cap, uint64_t val)
{
    if (*cnt == *cap) {
        *cap = (*cap == 0) ? 8 : (*cap << 1);
        *arr = (uint64_t *)realloc(*arr, *cap * sizeof(uint64_t));
    }
    (*arr)[(*cnt)++] = val;
}

/* sieve primes up to 'limit' (inclusive) and store into *primes, with count and capacity */
static void sieve_primes(uint64_t limit,
                        uint64_t **primes, size_t *primes_cnt, size_t *primes_cap)
{
    bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_prime) return;
    for (uint64_t i = 2; i <= limit; ++i) is_prime[i] = true;
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) is_prime[j] = false;
        }
    }
    *primes_cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) push_uint64(primes, primes_cnt, primes_cap, i);
    }
    free(is_prime);
}

/* build array of primes p where p+36 and p+72 are also prime, up to 'limit' */
static void build_pattern(uint64_t limit,
                          uint64_t *primes, size_t primes_cnt,
                          uint64_t **pattern, size_t *pattern_cnt, size_t *pattern_cap,
                          bool *is_prime)
{
    free(*pattern);
    *pattern = NULL;
    *pattern_cnt = 0;
    *pattern_cap = 0;
    for (size_t k = 0; k < primes_cnt; ++k) {
        uint64_t p = primes[k];
        if (p + 72 > limit) break;
        if (is_prime[p + 36] && is_prime[p + 72]) {
            push_uint64(pattern, pattern_cnt, pattern_cap, p);
        }
    }
}

/* Main function requested by the OEIS entry */
seq_t a(idx_t n)
{
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0, primes_cap = 0;

    static uint64_t *pattern = NULL;
    static size_t pattern_cnt = 0, pattern_cap = 0;

    static uint64_t sieve_limit = 0;

    if (n == 0) return (seq_t)0;  /* safety; offset is 1 */

    /* Ensure we have enough pattern primes for the requested index */
    while (pattern_cnt < (size_t)n) {
        /* Increase sieve limit (start small, double each time) */
        if (sieve_limit < 1000) sieve_limit = 1000;
        else sieve_limit <<= 1;   /* double */

        /* Build primes up to the current limit */
        sieve_primes(sieve_limit, &primes, &primes_cnt, &primes_cap);

        /* Create a quick prime lookup array */
        bool *is_prime = (bool *)calloc(sieve_limit + 1, sizeof(bool));
        if (!is_prime) return (seq_t)0;
        for (uint64_t i = 2; i <= sieve_limit; ++i) is_prime[i] = true;
        for (uint64_t i = 2; i * i <= sieve_limit; ++i) {
            if (is_prime[i]) {
                for (uint64_t j = i * i; j <= sieve_limit; j += i) is_prime[j] = false;
            }
        }

        /* Build or rebuild the pattern array */
        build_pattern(sieve_limit, primes, primes_cnt,
                      &pattern, &pattern_cnt, &pattern_cap, is_prime);

        free(is_prime);
    }

    return (seq_t)pattern[n - 1];
}
