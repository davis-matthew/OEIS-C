/*
 * A002731  Numbers k such that (k^2 + 1)/2 is prime.
 * https://oeis.org/A002731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   search over odd k: a(n) = n-th odd k such that (k^2 + 1)/2 is
 *            prime (deterministic Miller-Rabin, memoized)
 * Range:     u8 n <= 42, u16 n <= 4695, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a2731_isprime, a2731_mulmod, a2731_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a2731_isprime.h"
#include "../utils/a2731_mulmod.h"
#include "../utils/a2731_powmod.h"

static uint64_t *a2731_terms = NULL;
static idx_t a2731_count = 0;
static idx_t a2731_cap = 0;
static uint64_t a2731_next_k = 1;

seq_t a(idx_t n) {
    while (a2731_count < n) {
        uint64_t k = a2731_next_k;
        a2731_next_k += 2;
        uint64_t v = (k * k + 1) / 2;
        if (a2731_isprime(v)) {
            if (a2731_count == a2731_cap) {
                idx_t nc = a2731_cap ? a2731_cap * 2 : 1024;
                uint64_t *p = (uint64_t *)realloc(a2731_terms, (size_t)nc * sizeof(uint64_t));
                if (!p) return (seq_t)0;
                a2731_terms = p;
                a2731_cap = nc;
            }
            a2731_terms[a2731_count++] = k;
        }
    }
    return (seq_t)a2731_terms[n - 1];
}
