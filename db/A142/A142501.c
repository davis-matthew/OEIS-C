/*
 * A142501  Primes congruent to 41 mod 51.
 * https://oeis.org/A142501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 41 (mod 51)
 * Range:     u8 n <= 1, u16 n <= 201, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *prime_cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static idx_t next_k = 0;

/* Find the n-th prime ≡ 41 (mod 51) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* offset is 1, but guard for 0 */

    /* Generate primes until we have at least n elements */
    while (cache_size < n) {
        idx_t candidate = 41 + 51 * next_k;
        next_k++;

        if (is_prime(candidate)) {
            /* Ensure enough capacity */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                prime_cache = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            prime_cache[cache_size] = (seq_t)candidate;
            ++cache_size;
        }
    }
    return prime_cache[n - 1];
}
