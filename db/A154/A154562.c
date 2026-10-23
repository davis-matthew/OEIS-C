/*
 * A154562  Positive integers n such that the sum of the squares of all the
 *          substring decompositions of n is a multiple of n.
 * https://oeis.org/A154562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n‑th positive integer such that the sum of the
 *            squares of all distinct numeric decimal‑substrings of that
 *            integer is a multiple of the integer itself
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 33, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "../utils/cmp_seq_t.h"

/* check whether a given integer satisfies the definition */
static bool is_good_candidate(idx_t cand)
{
    /* guard against exceeding 64 bit, we only handle numbers that fit in uint64_t */
    if (cand > (idx_t)UINT64_MAX) return false;
    uint64_t n = (uint64_t)cand;

    /* convert integer to decimal string */
    char buf[32];
    int len = snprintf(buf, sizeof(buf), "%llu", (unsigned long long)n);
    if (len <= 0) return false;          /* should never happen */

    /* number of possible substrings = len*(len+1)/2 */
    size_t maxsubs = (size_t)len * (len + 1) / 2;
    seq_t *subvals = (seq_t *)malloc(maxsubs * sizeof(seq_t));
    if (!subvals) abort();

    /* generate all substrings and store their numeric values */
    size_t count = 0;
    for (int i = 0; i < len; ++i) {
        uint64_t val = 0;
        for (int j = i; j < len; ++j) {
            val = val * 10 + (uint64_t)(buf[j] - '0');
            subvals[count++] = (seq_t)val;
        }
    }

    /* sort to deduplicate */
    qsort(subvals, count, sizeof(seq_t), cmp_seq_t);

    /* sum of squares of distinct values */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < count; ) {
        seq_t cur = subvals[i];
        sum += cur * cur;
        size_t j = i + 1;
        while (j < count && subvals[j] == cur) ++j;
        i = j;
    }
    free(subvals);

    /* check divisibility */
    seq_t cand_seq = (seq_t)cand;
    return (sum % cand_seq) == (seq_t)0;
}

/* storage for already found terms of the sequence */
static seq_t *terms     = NULL;
static size_t  terms_len = 0;
static size_t  terms_cap = 0;
static idx_t   next_candidate = 1;

/* sequence function requested by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for non‑positive indices */

    /* generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        if (is_good_candidate(next_candidate)) {
            /* add to storage */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap << 1 : 4;
                seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!new_terms) abort();
                terms = new_terms;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return terms[(size_t)n - 1];
}
