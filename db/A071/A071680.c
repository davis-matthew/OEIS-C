/*
 * A071680  Primes that are the arithmetic mean of their prime predecessor and
 *          another prime.
 * https://oeis.org/A071680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) such that 2*prime(n)-prime(n-1) is prime. The
 *            sequence is defined for n ≥ 1, with a(1)=5
 * Range:     u8 n <= 7, u16 n <= 491, u32 n <= 492, u64 n <= 8941,
 *            u128 n <= 8941, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities (provided by the build) */
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t) */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t) */

/* Standard headers */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* ---------- Static state ---------- */
static seq_t *terms   = NULL; /* collected sequence values          */
static size_t term_sz = 0;    /* number of stored terms             */
static size_t cap     = 0;    /* current allocation size            */
static uint64_t prev_prime = 2; /* previous prime in the full sequence */

/* Append a new value to the sequence array */
static void append_term(seq_t val) {
    if (term_sz == cap) {
        size_t new_cap = (cap == 0) ? 16 : cap << 1;
        seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!new_arr) abort();              /* out of memory */
        terms = new_arr;
        cap   = new_cap;
    }
    terms[term_sz++] = val;
}

/* Generate terms until we have at least n of them */
static void generate_terms(idx_t n) {
    while (term_sz < n) {
        uint64_t cur = next_prime(prev_prime);          /* next prime after prev */
        seq_t test = 2 * (seq_t)cur - (seq_t)prev_prime;
        if (is_prime_big(test)) append_term((seq_t)cur);
        prev_prime = cur;
    }
}

/* ---------- Public function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined, but harmless */
    generate_terms(n);              /* ensure term n is available */
    return terms[n - 1];            /* 1‑based OEIS indexing */
}
