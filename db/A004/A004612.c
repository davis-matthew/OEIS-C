/*
 * A004612  Numbers that are divisible only by primes congruent to 2 mod 3.
 * https://oeis.org/A004612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th positive integer all of whose prime factors
 *            are congruent to 2 mod 3 (a(1) = 1)
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a004612_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/a004612_ok.h"

static uint64_t *a004612_terms = NULL;
static idx_t a004612_len = 0;
static idx_t a004612_cap = 0;
static uint64_t a004612_last = 0;

seq_t a(idx_t n) {
    while (a004612_len < n) {
        if (a004612_len == a004612_cap) {
            idx_t nc = a004612_cap ? a004612_cap * 2 : 1024;
            a004612_terms = (uint64_t *)realloc(a004612_terms, (size_t)nc * sizeof(uint64_t));
            a004612_cap = nc;
        }
        uint64_t k = a004612_last;
        do {
            k++;
        } while (!a004612_ok(k));
        a004612_terms[a004612_len++] = k;
        a004612_last = k;
    }
    return (seq_t)a004612_terms[n - 1];
}
