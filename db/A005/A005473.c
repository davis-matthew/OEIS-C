/*
 * A005473  Primes of form k^2 + 4.
 * https://oeis.org/A005473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search over odd k = 1, 3, 5, ...; a(n) is the n-th prime of the
 *            form k^2 + 4 (deterministic Miller-Rabin test)
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 4431, u64 n <= 4600,
 *            u128 n <= 4600, big any n
 * Helpers:   mr_isprime, mulmod_u64, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mr_isprime.h"
#include "../utils/mulmod_u64.h"
#include "../utils/powmod_u64.h"

static uint64_t *kcache = NULL;
static idx_t kcache_len = 0;
static idx_t kcache_cap = 0;
static uint64_t next_k = 1;

static void ensure_cache(idx_t n) {
    if (kcache_len >= n) return;
    if (kcache_cap < n) {
        idx_t cap = kcache_cap ? kcache_cap : 64;
        while (cap < n) cap *= 2;
        kcache = (uint64_t *)realloc(kcache, cap * sizeof(uint64_t));
        kcache_cap = cap;
    }
    while (kcache_len < n) {
        uint64_t k = next_k;
        next_k += 2;
        if (mr_isprime(k * k + 4)) kcache[kcache_len++] = k;
    }
}

seq_t a(idx_t n) {
    ensure_cache(n);
    uint64_t k = kcache[n - 1];
    return (seq_t)k * (seq_t)k + 4;
}
