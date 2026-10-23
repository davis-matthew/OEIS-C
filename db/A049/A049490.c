/*
 * A049490  a(n) and a(n)+64 both prime.
 * https://oeis.org/A049490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that p + 64 is also prime
 * Range:     u8 n <= 11, u16 n <= 701, u32 n <= 852, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

/* static storage for the sequence and the candidate */
static seq_t *terms = NULL;          /* array of terms */
static size_t terms_cap = 0;         /* capacity of the array */
static size_t terms_cnt = 0;         /* number of terms stored */
static seq_t next_candidate;         /* next odd number to start a search */
static bool initialized = false;     /* guard for one‑time initialization */

/* Allocate or grow the terms array */
static void grow_terms(size_t new_cap)
{
    if (!terms) {
        terms = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!terms) abort();
        terms_cap = new_cap;
        return;
    }
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    terms = tmp;
    terms_cap = new_cap;
}

/* Find the nth term, generating terms on demand. */
seq_t a(idx_t n)
{
    /* Lazily initialize static data */
    if (!initialized) {
        /* start searching from the first odd prime candidate */
        next_candidate = (seq_t)3;
        terms_cap = 128;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) abort();
        terms_cnt = 0;
        initialized = true;
    }

    /* Ensure the requested index fits in size_t for safe array access */
    if ((size_t)n > SIZE_MAX) {
        /* out of range – abort instead of undefined behaviour */
        abort();
    }

    /* Generate new terms until we have at least n of them */
    while (terms_cnt < (size_t)n) {
        /* Find the next prime candidate */
        seq_t p = next_candidate;
        while (!is_prime_big(p)) {
            p += (seq_t)2;      /* skip even numbers */
        }

        /* Check the twin‑prime‑offset condition */
        seq_t q = p + (seq_t)64;
        if (is_prime_big(q)) {
            /* Store the term, growing the array if necessary */
            if (terms_cnt >= terms_cap)
                grow_terms(terms_cap << 1);          /* double capacity */
            terms[terms_cnt++] = p;

            /* Advance the next starting point for the next search */
            next_candidate = p + (seq_t)2;
        } else {
            /* Continue searching from the prime just found */
            next_candidate = p + (seq_t)2;
        }
    }

    return terms[(size_t)n - 1];
}
