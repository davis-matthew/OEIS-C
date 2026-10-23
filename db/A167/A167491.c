/*
 * A167491  Members of A167490 sorted in ascending order.
 * https://oeis.org/A167491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth smallest integer such that the product of the
 *            lengths of consecutive runs of equal bits in its binary
 *            representation is unique among all smaller integers
 * Range:     u8 n <= 14, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   product_of_binary_run_lengths
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/product_of_binary_run_lengths.h"

/* Static state for memoization */
static seq_t *terms       = NULL; /* the sequence values  */
static size_t  term_cnt   = 0;    /* number of terms stored  */
static size_t  term_cap   = 0;    /* capacity of terms array */

static seq_t *seen        = NULL; /* sorted list of seen products */
static size_t  seen_cnt   = 0;    /* number of products seen   */
static size_t  seen_cap   = 0;    /* capacity of seen array   */

static seq_t  cur_val = (seq_t)0; /* next integer to examine */

/* Ensure we have at least n terms computed (n is 1‑based). */
static void ensure_terms(idx_t n)
{
    /* Grow capacity when needed */
    if (term_cap < 8) {
        term_cap = 8;
        terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
    }
    if (seen_cap < 8) {
        seen_cap = 8;
        seen = (seq_t *)realloc(seen, seen_cap * sizeof(seq_t));
    }

    while (term_cnt < (size_t)n) {
        seq_t prod = product_of_binary_run_lengths(cur_val);

        /* Binary search for prod in the sorted seen array. */
        size_t lo = 0, hi = seen_cnt;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (seen[mid] < prod)
                lo = mid + 1;
            else
                hi = mid;
        }
        /* lo is insertion point; check if prod already present. */
        if (lo < seen_cnt && seen[lo] == prod) {
            /* already seen, just move on */
            cur_val = cur_val + (seq_t)1;
            continue;
        }

        /* New product: record term and product. */
        if (term_cnt == term_cap) {
            term_cap <<= 1;
            terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
        }
        terms[term_cnt++] = cur_val;

        if (seen_cnt == seen_cap) {
            seen_cap <<= 1;
            seen = (seq_t *)realloc(seen, seen_cap * sizeof(seq_t));
        }
        /* Shift seen elements to make room at position lo. */
        for (size_t i = seen_cnt; i > lo; --i)
            seen[i] = seen[i - 1];
        seen[lo] = prod;
        ++seen_cnt;

        cur_val = cur_val + (seq_t)1;
    }
}

/* Public interface */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;          /* undefined for n <= 0; safe fallback */

    ensure_terms(n);
    return terms[(size_t)(n - (idx_t)1)];
}
