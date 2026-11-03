/*
 * A199995  Composite numbers whose multiplicative persistence is 5.
 * https://oeis.org/A199995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 670 (2026-11-03)
 * Helpers:   digit_product, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A199995: Composite numbers whose multiplicative persistence is 5.
   Each term is the next composite number whose multiplicative persistence equals 5.
   The function is implemented by a straightforward search with memoisation. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <limits.h>
#include "../utils/digit_product.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

static seq_t *cache = NULL;   /* cached terms */
static size_t cache_len  = 0; /* number of terms stored */
static size_t cache_cap  = 0; /* allocated capacity */
static seq_t scanning = 2;    /* next integer to test */

static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)
        abort();  /* out of memory */
    cache     = tmp;
    cache_cap = new_cap;
}

/* Return true if the multiplicative persistence of n is exactly 5 */
static bool has_persistence_5(seq_t n)
{
    seq_t x = n;
    int steps = 0;
    while (x >= (seq_t)10 && steps <= 5) {
        x = digit_product(x);
        ++steps;
    }
    return steps == 5;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    size_t target = (size_t)n;
    if (cache_len < target) {
        ensure_cache_capacity(target);
        while (cache_len < target) {
            if (scanning > (seq_t)1 && !isPrimeGeneric(scanning)) {
                if (has_persistence_5(scanning))
                    cache[cache_len++] = scanning;
            }
            ++scanning;
        }
    }
    return cache[target - 1];
}
