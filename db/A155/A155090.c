/*
 * A155090  Composites k such that composite(k) is odd.
 * https://oeis.org/A155090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th composite index k such that composite(k) is odd.
 *            Generation: incrementally build the composite number sequence,
 *            test each candidate index k for compositeness, and check whether
 *            the k‑th composite number is odd
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* dynamic array holding the list of composite numbers.
 * composites[i] == the (i+1)-th composite number (1‑based). */
static seq_t *composites = NULL;
static size_t composites_cnt = 0;
static size_t composites_cap = 0;
/* the next natural number that has not yet been tested for compositeness. */
static idx_t next_comp_start = 4;

/* Ensure that composites array contains at least `target` elements. */
static void ensure_composites_cnt(size_t target)
{
    while (composites_cnt < target) {
        idx_t m = next_comp_start;
        /* find the next composite number */
        while (m <= 3 || is_prime(m))
            ++m;
        if (composites_cnt >= composites_cap) {
            size_t new_cap = composites_cap ? composites_cap * 2 : 32;
            seq_t *tmp = (seq_t *)realloc(composites,
                                          new_cap * sizeof(seq_t));
            composites = tmp;
            composites_cap = new_cap;
        }
        composites[composites_cnt++] = (seq_t)m;
        next_comp_start = m + 1;
    }
}

/* dynamic array holding the sequence a(n). */
static seq_t *a_terms = NULL;
static size_t a_terms_cnt = 0;
static size_t a_terms_cap = 0;
/* the next integer to consider as a candidate index k. */
static idx_t k_scan = 4;

/* Ensure that a_terms array contains at least `n` terms. */
static void ensure_a_terms_n(idx_t n)
{
    while (a_terms_cnt < n) {
        /* Make sure we have the k‑th composite number available. */
        ensure_composites_cnt((size_t)k_scan);
        if (k_scan > 3 && !is_prime(k_scan)) {
            seq_t composite_k = composites[(size_t)(k_scan - 1)];
            if ((composite_k & 1) != 0) {
                if (a_terms_cnt >= a_terms_cap) {
                    size_t new_cap = a_terms_cap ? a_terms_cap * 2 : 32;
                    seq_t *tmp = (seq_t *)realloc(a_terms,
                                                  new_cap * sizeof(seq_t));
                    a_terms = tmp;
                    a_terms_cap = new_cap;
                }
                a_terms[a_terms_cnt++] = (seq_t)k_scan;
            }
        }
        ++k_scan;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* 1‑based sequence; undefined for 0 */
    ensure_a_terms_n(n);
    return a_terms[(size_t)(n - 1)];
}
