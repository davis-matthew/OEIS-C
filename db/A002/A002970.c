/*
 * A002970  Numbers k such that 4*k^2 + 9 is prime.
 * https://oeis.org/A002970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: a(n) = n-th k >= 1 such that 4*k^2 + 9 is prime
 *            (Miller-Rabin test)
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 960 (2026-11-03)
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

static uint64_t *a002970_terms = NULL;
static idx_t a002970_count = 0;
static idx_t a002970_cap = 0;
static uint64_t a002970_last_k = 0;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    while (a002970_count < n) {
        uint64_t k = ++a002970_last_k;
        if (is_prime_u64(4 * k * k + 9)) {
            if (a002970_count == a002970_cap) {
                idx_t nc = a002970_cap ? 2 * a002970_cap : 1024;
                a002970_terms = (uint64_t *)realloc(a002970_terms, (size_t)nc * sizeof(uint64_t));
                a002970_cap = nc;
            }
            a002970_terms[a002970_count++] = k;
        }
    }
    return (seq_t)a002970_terms[n - 1];
}
