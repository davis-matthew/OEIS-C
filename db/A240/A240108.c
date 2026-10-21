/*
 * A240108  Numbers with primitive root 20.
 * https://oeis.org/A240108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth positive integer such that 20 is a primitive root modulo that integer. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

/* forward declaration of helper */
static void grow_cache(size_t needed);

/* Test if 20 is a primitive root modulo x (gcd(x,20)=1). */
static bool is_primitive_root_20(uint64_t x)
{
    /* Compute Euler's totient φ(x). */
    uint64_t n = x;
    uint64_t phi = x;
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            while (n % p == 0)
                n /= p;
            phi = phi / p * (p - 1);
        }
    }
    if (n > 1)
        phi = phi / n * (n - 1);

    /* Compute the multiplicative order of 20 modulo x. */
    uint64_t ord = phi;
    uint64_t tmp = phi;

    /* Handle factor 2 separately. */
    if ((tmp & 1ULL) == 0) {
        while ((tmp & 1ULL) == 0)
            tmp >>= 1;
        while (ord % 2 == 0 && mod_pow((seq_t)20, ord / 2, (seq_t)x) == (seq_t)1)
            ord /= 2;
    }

    /* Remaining odd prime factors. */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            while (tmp % p == 0)
                tmp /= p;
            while (ord % p == 0 &&
                   mod_pow((seq_t)20, ord / p, (seq_t)x) == (seq_t)1)
                ord /= p;
        }
    }
    if (tmp > 1) {
        uint64_t p = tmp;
        while (ord % p == 0 &&
               mod_pow((seq_t)20, ord / p, (seq_t)x) == (seq_t)1)
            ord /= p;
    }

    return ord == phi;
}

/* Dynamic cache for generated sequence terms. */
static seq_t *seq_cache      = NULL;
static size_t cache_len      = 0;
static size_t cache_cap      = 0;
static uint64_t next_candidate = 3;  /* start from 3, the first term */

/* Grow the cache to at least 'needed' capacity. */
static void grow_cache(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = tmp;
        cache_cap = new_cap;
    }
}

/* OEIS sequence A240108: numbers with primitive root 20. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* unused; offset starts at 1 */

    /* Extend cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        uint64_t val = next_candidate++;

        /* gcd(val,20) == 1 -> val must be odd and not divisible by 5. */
        if ((val & 1ULL) == 0 || val % 5ULL == 0)
            continue;

        if (is_primitive_root_20(val)) {
            grow_cache(cache_len + 1);
            seq_cache[cache_len++] = (seq_t)val;
        }
    }

    return seq_cache[n - 1];
}
