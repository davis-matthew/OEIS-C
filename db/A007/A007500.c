/*
 * A007500  Primes whose reversal in base 10 is also prime (called
 *          "palindromic primes" by David Wells, although that name usually
 *          refers to A002385). Also called reversible primes.
 * https://oeis.org/A007500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, nice
 * Formula:   search: a(n) is the n-th prime p whose base-10 digit reversal is
 *            also prime
 * Range:     u8 n <= 25, u16 n <= 1050, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static uint64_t reverse_digits(uint64_t n) {
    uint64_t result = 0;
    while (n > 0) {
        result = result * 10 + (n % 10);
        n /= 10;
    }
    return result;
}

seq_t a(idx_t n) {
    static uint64_t *cache = 0;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    if (n == 0) return (seq_t)0;
    if (n <= cache_len) {
        return (seq_t)cache[n - 1];
    }

    if (n > cache_cap) {
        idx_t newcap = cache_cap ? cache_cap : 64;
        while (newcap < n) newcap *= 2;
        cache = (uint64_t *)realloc(cache, (size_t)newcap * sizeof(uint64_t));
        cache_cap = newcap;
    }

    uint64_t candidate = (cache_len > 0) ? cache[cache_len - 1] + 1 : 2;

    while (cache_len < n) {
        if (is_prime(candidate) && is_prime(reverse_digits(candidate))) {
            cache[cache_len++] = candidate;
        }
        candidate++;
    }

    return (seq_t)cache[n - 1];
}
