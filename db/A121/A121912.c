/*
 * A121912  Numbers k such that 10^k == 10 (mod k).
 * https://oeis.org/A121912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 6669, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of sequence A121912:
 *   a(n) = k where k is the n-th positive integer such that 10^k ≡ 10 (mod k).
 *   This is evaluated by brute‑force search using modular exponentiation.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

static seq_t *terms = NULL;   /* array of found terms */
static size_t  terms_cap = 0;
static size_t  terms_cnt = 0; /* number of terms stored */
static idx_t   tested   = 0;  /* largest k that has been tested */

/* Add a new term k to the list */
static void add_term(idx_t k) {
    if (terms_cnt == terms_cap) {
        size_t new_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
        seq_t *new_arr = (seq_t *) realloc(terms, new_cap * sizeof(seq_t));
        if (!new_arr) exit(EXIT_FAILURE); /* failed to allocate */
        terms = new_arr;
        terms_cap = new_cap;
    }
    terms[terms_cnt++] = (seq_t) k;
}

/* Test whether k satisfies 10^k ≡ 10 (mod k) */
static bool good_k(idx_t k) {
    /* Base 10 as seq_t */
    const seq_t TEN = (seq_t)10;
    /* Compute 10^k mod k */
    seq_t mod_res = mod_pow(TEN, (uint64_t)k, (seq_t)k);
    /* Compute 10 mod k */
    seq_t ten_mod = (seq_t)10 % (seq_t)k;
    return mod_res == ten_mod;
}

/* Extend the list until it contains at least n terms */
static void ensure_n_terms(size_t n) {
    if (terms_cnt >= n) return;
    for (idx_t k = tested + 1; ; ++k) {
        if (good_k(k)) add_term(k);
        tested = k;
        if (terms_cnt >= n) break;
    }
}

/* Public entry point required by the exercise */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n=0, return 0 */
    /* n is 1‑based */
    size_t idx = (size_t)n - 1;
    ensure_n_terms(idx + 1);
    return terms[idx];
}
