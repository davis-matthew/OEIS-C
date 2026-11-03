/*
 * A132982  The non-prime-power "antimutinous" numbers. (Antimutinous numbers
 *          are those integers m, m >1, where m/p^k < p, p = largest prime
 *          divisor of m, p^k = largest power of p that divides m.)
 * https://oeis.org/A132982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth non‑prime‑power antimutinous number (antimutinous: m
 *            / p^k < p, with p the largest prime divisor, p^k the highest
 *            power of p dividing m). This implementation enumerates integers
 *            and stores them in a static cache, so that calls with arbitrary
 *            n are answered efficiently. It works for any...
 * Range:     u8 n <= 136, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9699 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: test if m (uint64_t) is an antimutinous non‑prime‑power number. */
static bool is_antimutinous_np(uint64_t m)
{
    if (m < 6)  /* smallest covered term is 6 */
        return false;

    uint64_t x = m;
    uint64_t largest_prime = 0;
    uint64_t power_of_largest = 1;

    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            uint64_t prod = 1;
            while (x % p == 0) {
                x /= p;
                prod *= p;
            }
            if (p > largest_prime) {
                largest_prime = p;
                power_of_largest = prod;
            }
        }
    }
    if (x > 1) {            /* remaining factor > sqrt(original m) */
        if (x > largest_prime) {
            largest_prime = x;
            power_of_largest = x;   /* exponent 1 */
        } else if (x == largest_prime) {
            /* should not happen, already processed above */
        }
    }

    /* If after removing the largest prime power nothing remains, m is a prime power. */
    uint64_t remaining = m / power_of_largest;
    if (remaining == 1)
        return false;       /* prime power, excluded */

    return remaining < largest_prime;
}

/* Cache for computed terms. */
static seq_t *cache = NULL;
static size_t cache_size = 0; /* number of terms stored */
static size_t cache_cap  = 0; /* allocated capacity */
static uint64_t next_candidate = 2;   /* next integer to test */

/* Ensure cache contains at least n terms. */
static void ensure_cache_up_to(idx_t n)
{
    if (cache_size >= n)
        return;

    /* Extend cache capacity as needed. */
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = mem_new(seq_t, cache_cap);
    }

    while (cache_size < n) {
        while (!is_antimutinous_np(next_candidate))
            ++next_candidate;

        /* store current candidate */
        if (cache_size >= cache_cap) {
            cache_cap *= 2;
            cache = mem_resize(seq_t, cache, cache_cap);
        }
        cache[cache_size++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/* Main sequence function requested by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)       /* offset is 1; a(0) irrelevant, return 0 */
        return (seq_t)0;
    ensure_cache_up_to(n);
    return cache[n - 1];
}
