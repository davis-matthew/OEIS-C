/*
 * A002328  Numbers k such that k^2 - k - 1 is prime.
 * https://oeis.org/A002328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: a(n) = n-th k >= 2 with k^2 - k - 1 prime (deterministic
 *            Miller-Rabin, memoized)
 * Range:     u8 n <= 105, u16 n <= 11628, u32 n <= 60000, u64 n <= 60000,
 *            u128 n <= 60000, big any n
 * Validated: standard, pass, 81 terms through n = 9404 (2026-11-03)
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

static uint64_t *a2328_terms = NULL;
static idx_t a2328_count = 0;
static idx_t a2328_cap = 0;
static uint64_t a2328_last_k = 1;

seq_t a(idx_t n) {
    while (a2328_count < n) {
        uint64_t k = ++a2328_last_k;
        uint64_t v = k * (k - 1) - 1;
        if (is_prime_u64(v)) {
            if (a2328_count == a2328_cap) {
                idx_t nc = a2328_cap ? a2328_cap * 2 : 1024;
                uint64_t *p = (uint64_t *)realloc(a2328_terms, nc * sizeof(uint64_t));
                if (!p) return 0;
                a2328_terms = p;
                a2328_cap = nc;
            }
            a2328_terms[a2328_count++] = k;
        }
    }
    return (seq_t)a2328_terms[n - 1];
}
