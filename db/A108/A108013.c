/*
 * A108013  Primes p such that p + 2 and p*(p + 2) + 2 are primes.
 * https://oeis.org/A108013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 70, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A108013: primes p such that p+2 and p*(p+2)+2 are prime; computed by incremental search. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime.h"

static seq_t *terms = NULL;
static idx_t terms_cap = 0;
static idx_t terms_size = 0;
static uint64_t next_candidate = 9;  /* next p to test after the first two terms */

static inline void add_term(uint64_t p) {
    if (terms_size == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms_cap = new_cap;
    }
    terms[terms_size++] = (seq_t)p;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined, but avoid UB */
    while (terms_size < n) {
        if (terms_size == 0) {
            add_term(3);
            continue;
        }
        if (terms_size == 1) {
            add_term(5);
            continue;
        }
        /* search for the next valid prime */
        for (uint64_t p = next_candidate; ; p += 10) {
            if (!is_prime(p))
                continue;
            if (!is_prime(p + 2))
                continue;
            unsigned __int128 prod = (unsigned __int128)p * (p + 2) + 2;
            if (prod > UINT64_MAX)
                break;   /* sequence would exceed 64‑bit range – not expected for n up to 10000 */
            if (is_prime((uint64_t)prod)) {
                add_term((uint64_t)p);
                next_candidate = p + 10;
                break;
            }
        }
    }
    return terms[n - 1];
}
