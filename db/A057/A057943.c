/*
 * A057943  Numbers k such that the smallest palindromic pseudoprime to base k
 *          is 101101.
 * https://oeis.org/A057943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 3026, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_pal32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find integer k such that the smallest palindromic
   Fermat pseudoprime to base k is 101101. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_pal32.h"

/* ------------------------------------------------------------------ */
/*  Helper: modular exponentiation (base^exp mod mod).  64‑bit only.  */
static uint64_t modpow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1ULL % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/*  Helper: greatest common divisor for 64‑bit integers.               */
static uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Helper: trial division primality test for n <= 101100.             */
static int is_prime_small(uint32_t n)
{
    if (n < 2) return 0;
    if (n % 2 == 0) return n == 2;
    for (uint32_t i = 3; i * i <= n; i += 2)
        if (n % i == 0)
            return 0;
    return 1;
}

/* ------------------------------------------------------------------ */
/*  Pre‑computed list of composite palindromes < 101101.               */
static uint32_t *pal_comp = NULL;
static size_t pal_size = 0, pal_cap = 0;
static int pal_ready = 0;

static void build_pal_comps(void)
{
    if (pal_ready) return;
    for (uint32_t n = 1; n < 101101; ++n) {
        if (!is_prime_small(n) && is_pal32(n)) {
            if (pal_size == pal_cap) {
                size_t new_cap = pal_cap ? pal_cap * 2 : 64;
                pal_comp = (uint32_t*)realloc(pal_comp, new_cap * sizeof(uint32_t));
                pal_cap = new_cap;
            }
            pal_comp[pal_size++] = n;
        }
    }
    pal_ready = 1;
}

/* ------------------------------------------------------------------ */
/*  Sequence cache.                                                     */
static seq_t *cache = NULL;
static size_t cache_size = 0, cache_cap = 0;
static uint64_t next_k = 2;      /* current candidate base */

static void cache_push(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* ------------------------------------------------------------------ */
/*  Main function: a(n)                                         */
seq_t a(idx_t n)
{
    /* Prepare data once. */
    build_pal_comps();

    /* Expand cache until it contains at least n terms. */
    while (cache_size < (size_t)n) {
        /* Skip bases that share a prime factor with 101101. */
        if (gcd64(next_k, 101101ULL) != 1) {
            ++next_k;
            continue;
        }

        int ok = 1;

        /* Check smaller palindromic composites. */
        for (size_t i = 0; i < pal_size; ++i) {
            uint32_t m = pal_comp[i];
            uint64_t r = modpow(next_k % m, (uint64_t)(m - 1), (uint64_t)m);
            if (r == 1ULL) {
                ok = 0;
                break;
            }
        }

        /* Check that 101101 itself is the first success. */
        if (ok) {
            uint64_t r = modpow(next_k % 101101ULL, 101100ULL, 101101ULL);
            if (r != 1ULL)
                ok = 0;
        }

        if (ok) {
            cache_push((seq_t)next_k);
        }
        ++next_k;
    }

    return cache[n - 1];
}
