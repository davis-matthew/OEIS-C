/*
 * A013930  Sum of first a(n) squarefrees is squarefree.
 * https://oeis.org/A013930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A013930: a(n) = the index m such that the sum of the first m
 * squarefree numbers is itself squarefree.
 * The function generates squarefree numbers on demand and maintains
 * a running sum.  Whenever that sum is squarefree, the current index
 * m is stored as a term a(k).  The function works for calls in any
 * order and precomputes only as much as required. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_smallest_prime_factor.h"

/* Local routine that tests whether a uint64_t number is square‑free. */
static bool is_squarefree_idx(uint64_t n) {
    if (n <= 1) return true;          /* 0 and 1 are square‑free by convention */
    uint64_t t = n;
    while (t > 1) {
        uint64_t p = _smallest_prime_factor(t);
        int exp = 0;
        while (t % p == 0) {
            t /= p;
            exp++;
            if (exp >= 2) return false;
        }
    }
    return true;
}

seq_t a(idx_t n) {
    /* static state to allow calls in any order */
    static bool initialized = false;
    static seq_t *terms = NULL;   /* array of found a(k) values */
    static size_t terms_cap = 0;  /* capacity of terms array */
    static idx_t terms_len = 0;   /* number of terms currently stored */

    static idx_t processed_count = 0;   /* number of squarefree numbers processed */
    static uint64_t sum = 0;            /* running sum of processed squarefrees */
    static uint64_t next_candidate = 1; /* next integer to test for squarefree */

    if (!initialized) {
        initialized = true;
    }

    /* generate terms until we have at least n of them */
    while (terms_len < n) {
        /* find next integer that is squarefree */
        uint64_t m = next_candidate;
        while (!is_squarefree_idx(m)) {
            ++m;
        }

        /* Process this squarefree integer */
        ++processed_count;
        sum += m;

        if (is_squarefree_idx(sum)) {
            /* grow terms array if needed */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len] = (seq_t)processed_count; /* record a(k) */
            ++terms_len;
        }

        next_candidate = m + 1; /* next integer to test in next loop */
    }

    return terms[n - 1];
}
