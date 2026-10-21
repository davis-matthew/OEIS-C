/*
 * A097220  Numbers n such that pi(n) = product of digits of n.
 * https://oeis.org/A097220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, more, nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   digit_product_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097220: numbers n such that pi(n) = product of digits of n. */
/* This implementation scans all integers up to a fixed limit (5 000 000),
   pre‑computes a prime sieve, counts the primes seen so far, and
   collects all n with digit product equal to that count.  The list
   is then accessible by a(idx_t n) for arbitrary n ≥ 1.  The sequence
   is finite (the largest term is 2 475 998). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_product_uint64.h"

/* External integer types supplied by the build. */
extern seq_t a(idx_t n);

/* Dynamic array for the found terms. */
static seq_t *terms = NULL;
static size_t  terms_len  = 0;
static size_t  terms_cap  = 0;

/* Append a term to the dynamic array, allocating more space if needed. */
static void append_term(seq_t v)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap == 0 ? 8 : terms_cap * 2;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);           /* out of memory */
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = v;
}

/* Pre‑compute all terms up to LIMIT.  The limit is chosen larger than the
   largest known term (2 475 998). */
#define LIMIT 5000000ULL

static void init_terms(void)
{
    /* Sieve of Eratosthenes to mark primes up to LIMIT. */
    size_t sz = (size_t)LIMIT + 1;
    char *is_prime = (char *)calloc(sz, sizeof(char));
    if (!is_prime) exit(1);
    for (size_t i = 2; i < sz; ++i) is_prime[i] = 1;
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i) is_prime[j] = 0;
        }
    }

    /* Scan all numbers, keeping a running count of primes seen so far. */
    unsigned long long prime_count = 0;
    for (size_t n = 1; n <= LIMIT; ++n) {
        if (is_prime[n]) ++prime_count;
        unsigned long long prod = digit_product_uint64((unsigned long long)n);
        if (prime_count == prod) {
            append_term((seq_t)n);
        }
    }

    free(is_prime);
}

/* Public entry point required by the evaluation system. */
seq_t a(idx_t n)
{
    /* Lazy initialization of the terms array. */
    if (!terms) init_terms();

    if (n == 0) return 0;
    if ((size_t)n <= terms_len) return terms[n - 1];
    return 0;   /* sequence is finite */
}
