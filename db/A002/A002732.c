/*
 * A002732  Numbers k such that (4*k^2 + 1)/5 is prime.
 * https://oeis.org/A002732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: a(n) = n-th k >= 1 with k == 1 or 4 (mod 5) such that
 *            (4*k^2 + 1)/5 is prime (deterministic Miller-Rabin)
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 995 (2026-11-03)
 * Helpers:   is_prime_u64, mulmod, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/powmod_u64.h"

static uint64_t *a002732_terms = NULL;
static idx_t a002732_count = 0;
static idx_t a002732_cap = 0;
static uint64_t a002732_next_k = 1;

seq_t a(idx_t n) {
    while (a002732_count < n) {
        uint64_t k = a002732_next_k++;
        uint64_t r = k % 5;
        if (r != 1 && r != 4) continue;
        uint64_t v = (4 * k * k + 1) / 5;
        if (is_prime_u64(v)) {
            if (a002732_count == a002732_cap) {
                idx_t nc = a002732_cap ? a002732_cap * 2 : 1024;
                a002732_terms = (uint64_t *)realloc(a002732_terms, nc * sizeof(uint64_t));
                a002732_cap = nc;
            }
            a002732_terms[a002732_count++] = k;
        }
    }
    return (seq_t)a002732_terms[n - 1];
}
