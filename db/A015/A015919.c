/*
 * A015919  Positive integers k such that 2^k == 2 (mod k).
 * https://oeis.org/A015919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that 2^k == 2 (mod k)
 * Range:     u8 n <= 55, u16 n <= 6607, u32 n <= 9162, u64 n <= 9162,
 *            u128 n <= 9162, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

static seq_t *terms    = NULL; /* stored sequence values */
static size_t  terms_len = 0;   /* number of values currently stored */
static size_t  terms_cap = 0;   /* allocated capacity of the array */
static uint64_t next_candidate = 1; /* candidate to test next */

static void grow_terms(size_t new_len)
{
    if (new_len <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    while (new_cap < new_len)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) {
        free(terms);
        exit(EXIT_FAILURE);
    }
    terms = tmp;
    terms_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Ensure the storage is initialised */
    if (terms_len == 0 && terms_cap == 0) {
        terms = NULL;
    }

    /* Generate terms until we have at least n of them */
    while (terms_len < n) {
        /* Skip even candidates greater than 2 – they can never satisfy 2^k ≡ 2 (mod k) */
        if (next_candidate > 2 && !(next_candidate & 1ULL)) {
            ++next_candidate;
            continue;
        }

        seq_t cand = (seq_t)next_candidate;
        seq_t rhs  = (seq_t)(2 % next_candidate);
        seq_t powmod = mod_pow((seq_t)2, next_candidate, cand);

        if (powmod == rhs) {
            grow_terms(terms_len + 1);
            terms[terms_len++] = cand;
        }

        ++next_candidate;
    }

    return terms[n - 1];
}
