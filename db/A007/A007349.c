/*
 * A007349  Primes with both 10 and -10 as primitive root.
 * https://oeis.org/A007349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over primes p and keep those where both 10 and -10 (mod
 *            p) are primitive roots
 * Range:     u8 n <= 11, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   a007349_both_roots, a007349_powmod, a007349_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/a007349_both_roots.h"
#include "../utils/a007349_powmod.h"
#include "../utils/a007349_prime.h"

static uint64_t *a007349_cache = NULL;
static uint64_t a007349_len = 0;
static uint64_t a007349_cap = 0;
static uint64_t a007349_next = 3;

seq_t a(idx_t n) {
    while (a007349_len < n) {
        uint64_t p = a007349_next++;
        if (p > 5 && a007349_prime(p) && a007349_both_roots(p)) {
            if (a007349_len == a007349_cap) {
                uint64_t nc = a007349_cap ? a007349_cap * 2 : 1024;
                a007349_cache = (uint64_t *)realloc(a007349_cache, nc * sizeof(uint64_t));
                a007349_cap = nc;
            }
            a007349_cache[a007349_len++] = p;
        }
    }
    return (seq_t)a007349_cache[n - 1];
}
