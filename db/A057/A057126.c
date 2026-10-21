/*
 * A057126  Numbers k such that 2 is a square mod k.
 * https://oeis.org/A057126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A057126: numbers k such that 2 is a square mod k
 * (= k not a multiple of 4, and all odd prime factors of k are ≡ ±1 (mod 8) ) */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;
static idx_t next_candidate = 1;   /* next integer to test */

static bool is_valid(idx_t k)
{
    if (k % 4 == 0) return false;   /* multiples of 4 are excluded */
    if (k == 1) return true;        /* 1 is always valid */

    uint64_t n = (uint64_t)k;
    /* factor_u64 fills arrays with prime factors and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        if (p == 2) {
            /* at most one 2 can appear, because k%4!=0 */
            continue;
        }
        if ((p % 8ULL) != 1ULL && (p % 8ULL) != 7ULL)
            return false;
    }
    return true;
}

static void ensure_cache(idx_t n)
{
    if (cache_size >= (size_t)n) return;

    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    while (cache_size < (size_t)n) {
        if (is_valid(next_candidate)) {
            if (cache_size == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but guard anyway */
    ensure_cache(n);
    return cache[n - 1];
}
