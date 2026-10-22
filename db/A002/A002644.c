/*
 * A002644  Numbers k such that (k^2 + k + 1)/21 is prime.
 * https://oeis.org/A002644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: k == 4 or 16 (mod 21) such that (k^2+k+1)/21 is prime
 *            (Miller-Rabin)
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   s2644_isprime, s2644_mulmod, s2644_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/s2644_isprime.h"
#include "../utils/s2644_mulmod.h"
#include "../utils/s2644_powmod.h"

static uint64_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static uint64_t next_k = 4;

seq_t a(idx_t n) {
    while (cache_len < n) {
        uint64_t k = next_k;
        uint64_t p = (k * k + k + 1) / 21;
        /* k mod 21 alternates 4, 16: steps +12, +9 */
        next_k += (k % 21 == 4) ? 12 : 9;
        if (s2644_isprime(p)) {
            if (cache_len == cache_cap) {
                idx_t nc = cache_cap ? cache_cap * 2 : 1024;
                cache = (uint64_t *)realloc(cache, (size_t)nc * sizeof(uint64_t));
                cache_cap = nc;
            }
            cache[cache_len++] = k;
        }
    }
    return (seq_t)cache[n - 1];
}
