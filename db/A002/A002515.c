/*
 * A002515  Lucasian primes: p == 3 (mod 4) with 2*p+1 prime.
 * https://oeis.org/A002515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   search, in increasing order, for primes p == 3 (mod 4) where
 *            2*p+1 is also prime (deterministic Miller-Rabin test); found
 *            terms are cached
 * Range:     u8 n <= 9, u16 n <= 415, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9654 (2026-11-03)
 * Helpers:   is_prime_u64, mulmod, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/powmod_u64.h"

static uint64_t *a002515_cache = NULL;
static idx_t a002515_len = 0, a002515_cap = 0;
static uint64_t a002515_next = 3;

seq_t a(idx_t n) {
    while (a002515_len < n) {
        uint64_t p = a002515_next;
        a002515_next += 4;
        if (is_prime_u64(p) && is_prime_u64(2 * p + 1)) {
            if (a002515_len == a002515_cap) {
                idx_t nc = a002515_cap ? 2 * a002515_cap : 1024;
                uint64_t *t = (uint64_t *)realloc(a002515_cache, (size_t)nc * sizeof(uint64_t));
                if (!t) return (seq_t)0;
                a002515_cache = t;
                a002515_cap = nc;
            }
            a002515_cache[a002515_len++] = p;
        }
    }
    return (seq_t)a002515_cache[n - 1];
}
