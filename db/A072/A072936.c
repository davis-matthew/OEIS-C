/*
 * A072936  Primes p that do not divide 2^x+1 for any x>=1.
 * https://oeis.org/A072936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes p such that p never divides 2^x+1 for any x ≥ 1.
   The function lazily finds such primes and caches them. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

/* Return true if the multiplicative order of 2 modulo p is odd. */
static inline bool has_odd_order(uint64_t p)
{
    if (p == 2) return true;
    uint64_t ord = p - 1;
    uint64_t m   = p - 1;
    for (uint64_t q = 2; q * q <= m; ++q) {
        if (m % q == 0) {
            while (m % q == 0) m /= q;
            while (ord % q == 0 &&
                   mod_pow((seq_t)2, ord / q, p) == (seq_t)1)
                ord /= q;
        }
    }
    if (m > 1) {
        uint64_t q = m;
        while (ord % q == 0 &&
               mod_pow((seq_t)2, ord / q, p) == (seq_t)1)
            ord /= q;
    }
    return (ord & 1) == 1;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t current = 2;

    while (cache_len < (size_t)n) {
        if (is_prime((idx_t)current)) {
            if (has_odd_order(current)) {
                if (cache_len + 1 > cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    if (new_cap < cache_len + 1) new_cap = cache_len + 1;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)current;
            }
        }
        ++current;
    }
    return cache[(size_t)n - 1];
}
