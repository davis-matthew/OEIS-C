/*
 * A007700  Numbers n such that n, 2n+1, and 4n+3 all prime.
 * https://oeis.org/A007700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- a(n) is the n-th prime p such that 2p+1 and 4p+3 are
 *            also prime (for p > 5, p == 11 or 29 (mod 30))
 * Range:     u8 n <= 6, u16 n <= 156, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a7700_isprime, a7700_mulmod, a7700_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a7700_isprime.h"
#include "../utils/a7700_mulmod.h"
#include "../utils/a7700_powmod.h"

static uint64_t *a7700_terms = NULL;
static idx_t a7700_len = 0;
static idx_t a7700_cap = 0;
static uint64_t a7700_next = 0;

static void a7700_push(uint64_t v) {
    if (a7700_len == a7700_cap) {
        idx_t nc = a7700_cap ? a7700_cap * 2 : 64;
        a7700_terms = (uint64_t *)realloc(a7700_terms, (size_t)nc * sizeof(uint64_t));
        a7700_cap = nc;
    }
    a7700_terms[a7700_len++] = v;
}

seq_t a(idx_t n) {
    if (a7700_len == 0) {
        a7700_push(2);
        a7700_push(5);
        a7700_next = 11;
    }
    while (a7700_len < n) {
        uint64_t p = a7700_next;
        /* candidates are 11 and 29 mod 30 */
        a7700_next += (p % 30 == 11) ? 18 : 12;
        if (a7700_isprime(p) && a7700_isprime(2 * p + 1) && a7700_isprime(4 * p + 3))
            a7700_push(p);
    }
    return (seq_t)a7700_terms[n - 1];
}
