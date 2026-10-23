/*
 * A124668  Numbers that together with their prime factors contain every digit
 *          exactly once.
 * https://oeis.org/A124668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) is the n‑th positive integer such that the decimal digits
 *            of the integer together with the digits of its distinct prime
 *            factors form a permutation of 0‑9
 * Range:     u8 none, u16 n <= 5, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static bool primes_ready = false;

static const uint16_t FULL_MASK = 0x3FF; /* bits 0‑9 set */

/* add the decimal digits of x to mask; return false if a duplicate digit is found */
static bool add_digits(uint64_t x, uint16_t *mask) {
    if (x == 0) {
        unsigned d = 0;
        unsigned bit = 1u << d;
        if (*mask & bit) return false;
        *mask |= bit;
        return true;
    }
    while (x > 0) {
        unsigned d = (unsigned)(x % 10ULL);
        unsigned bit = 1u << d;
        if (*mask & bit) return false;
        *mask |= bit;
        x /= 10ULL;
    }
    return true;
}

/* test whether n satisfies the digit permutation property */
static bool good_candidate(uint64_t n) {
    uint16_t mask = 0;
    if (!add_digits(n, &mask)) return false;
    uint64_t tmp = n;
    /* ensure prime list is available */
    if (!primes_ready) {
        build_primes_upto((idx_t)1000000, &primes, &primes_cnt);
        primes_ready = true;
    }
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            if (!add_digits(p, &mask)) return false;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) { /* remaining prime factor */
        if (!add_digits(tmp, &mask)) return false;
    }
    return mask == FULL_MASK;
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t current_candidate = 1u;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* behaviour below the offset is irrelevant */
    /* generate until we have n terms cached */
    while (cache_len < (size_t)n) {
        while (!good_candidate(current_candidate)) {
            ++current_candidate;
        }
        /* ensure capacity in cache */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap < 8 ? 8 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)current_candidate;
        ++current_candidate;
    }
    return cache[(size_t)n - 1];
}
