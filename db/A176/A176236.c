/*
 * A176236  Numbers n such that one of nonnegative nonprime(n)-2 or
 *          nonnegative nonprime(n)+2 is prime.
 * https://oeis.org/A176236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A176236 by testing nonnegative nonprime(n)-2 and +2 for primality.
   The first k terms are stored in a static cache, so repeated calls are fast. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */

static seq_t *nonprimes = NULL;   /* nonnegative nonprime numbers, 0‑based index */
static idx_t   nonprimes_len = 0;
static size_t  nonprimes_cap = 0;
static seq_t   next_candidate = 0;   /* smallest integer to test for nonprime */

static seq_t *terms = NULL;        /* sequence A176236 values */
static idx_t   terms_len = 0;
static size_t  terms_cap = 0;
static idx_t   next_test_n = 1;    /* next integer n to evaluate */

/* Append to the nonprimes array, growing if necessary. */
static void push_nonprime(seq_t val) {
    if (nonprimes_len == nonprimes_cap) {
        size_t new_cap = nonprimes_cap ? nonprimes_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(nonprimes, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        nonprimes = tmp;
        nonprimes_cap = new_cap;
    }
    nonprimes[nonprimes_len++] = val;
}

/* Ensure that nonprimes array holds at least n elements (n≥1). */
static void ensure_nonprimes(idx_t n) {
    while (nonprimes_len < n) {
        seq_t cand = next_candidate;
        while (is_prime_big(cand)) cand++;
        push_nonprime(cand);
        next_candidate = cand + 1;
    }
}

/* Append to the terms array, growing if necessary. */
static void push_term(seq_t val) {
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* Produce terms up to index k (1‑based). */
static void ensure_terms(idx_t k) {
    while (terms_len < k) {
        ensure_nonprimes(next_test_n);
        seq_t val = nonprimes[next_test_n - 1];
        bool prime_minus = false;
        if (val >= (seq_t)2) prime_minus = is_prime_big(val - 2);
        bool prime_plus = is_prime_big(val + 2);
        if (prime_minus ^ prime_plus) push_term((seq_t)next_test_n);
        ++next_test_n;
    }
}

/* Public function required by the problem. */
seq_t a(idx_t n) {
    ensure_terms(n);
    return terms[n - 1];
}
