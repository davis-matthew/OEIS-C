/*
 * A105881  Primes for which -9 is a primitive root.
 * https://oeis.org/A105881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 931 (2026-11-03)
 * Helpers:   is_prime_u64, mem, mulmod, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes for which -9 is a primitive root (A105881) */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/powmod_u64.h"
#include "../utils/mem.h"

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

/* Test whether a prime p has –9 as a primitive root modulo p. */
static bool is_primitive_minus9(uint64_t p)
{
    if (p == 2) return true;          /* p‑1 = 1, -9 ≡ 1 (mod 2) */
    if (p == 3) return false;         /* -9 ≡ 0 (mod 3) -> not coprime */

    /* Compute g = -9 mod p. */
    uint64_t g = (p - (9 % p)) % p;
    if (g == 0) return false;         /* p divides 9 */

    uint64_t m = p - 1;
    uint64_t tmp = m;
    uint64_t factors[64];
    size_t nf = 0;
    for (uint64_t d = 2; d * d <= tmp; ++d) {
        if (tmp % d == 0) {
            factors[nf++] = d;
            while (tmp % d == 0) tmp /= d;
        }
    }
    if (tmp > 1) factors[nf++] = tmp;

    for (size_t i = 0; i < nf; ++i) {
        if (powmod_u64(g, m / factors[i], p) == 1) return false;
    }
    return true;
}

/* 128‑bit modular multiplication for 64‑bit operands. */

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of accepted primes */
    static idx_t cache_size = 0;
    static idx_t cache_cap  = 0;
    static uint64_t cand = 3;      /* next odd candidate to test */
    static bool initialized = false;

    if (!initialized) {
        cache_cap   = 64;
        cache       = seq_malloc(cache_cap);
        cache[0]    = (seq_t)2;     /* first element is 2 */
        cache_size  = 1;
        initialized = true;
    }

    /* Extend caching until we have at least n terms. */
    while (cache_size < n) {
        if (is_prime_u64(cand) && is_primitive_minus9(cand)) {
            if (cache_size == cache_cap) {
                cache_cap <<= 1;                /* double capacity */
                cache = seq_realloc(cache, cache_cap);
            }
            cache[cache_size++] = (seq_t)cand;
        }
        cand += 2;   /* skip even numbers */
    }

    return cache[n - 1];
}
