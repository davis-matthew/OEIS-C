/*
 * A050011  Numbers k such that A050008(k) > A050008(k+1).
 * https://oeis.org/A050011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 such that A050008(k) > A050008(k+1); computed
 *            by scanning A050008 (A050008(1)=1; A050008(m) =
 *            floor(A050008(m-1)/2) if positive and new, else 6*A050008(m-1))
 * Range:     u8 n <= 26, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A050008_seq = NULL;
static size_t A050008_len = 0;
static size_t A050008_cap = 0;

static void A050008_grow(size_t needed)
{
    if (needed <= A050008_cap)
        return;
    size_t new_cap = A050008_cap ? A050008_cap * 2 : 64;
    if (new_cap < needed)
        new_cap = needed;
    A050008_seq = mem_resize(seq_t, A050008_seq, new_cap);
    A050008_cap = new_cap;
}

static void A050008_compute(idx_t n)
{
    A050008_grow((size_t)n + 1);
    if (A050008_len == 0) {
        A050008_seq[0] = (seq_t)0;
        A050008_seq[1] = (seq_t)1;
        A050008_len = 1;
    }
    for (idx_t i = (idx_t)A050008_len + 1; i <= n; ++i) {
        seq_t prev = A050008_seq[(size_t)i - 1];
        seq_t cand = prev / (seq_t)2;
        bool use_cand = false;
        if (cand > (seq_t)0) {
            bool found = false;
            for (idx_t j = 1; j < i; ++j) {
                if (A050008_seq[(size_t)j] == cand) {
                    found = true;
                    break;
                }
            }
            if (!found)
                use_cand = true;
        }
        A050008_seq[(size_t)i] = use_cand ? cand : prev * (seq_t)6;
    }
    A050008_len = (size_t)n;
}

static seq_t A050008_a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;
    if ((size_t)n > A050008_len)
        A050008_compute(n);
    return A050008_seq[(size_t)n];
}

static idx_t *desc_terms = NULL;
static size_t desc_count = 0;
static size_t desc_cap = 0;
static idx_t desc_next = 1;   /* next k to examine */

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;
    while ((idx_t)desc_count < n) {
        idx_t k = desc_next;
        seq_t x = A050008_a(k);
        seq_t y = A050008_a(k + 1);
        if (x > y) {
            if (desc_count >= desc_cap) {
                size_t nc = desc_cap ? desc_cap * 2 : 64;
                desc_terms = mem_resize(idx_t, desc_terms, nc);
                desc_cap = nc;
            }
            desc_terms[desc_count++] = k;
        }
        desc_next = k + 1;
    }
    return (seq_t)desc_terms[(size_t)n - 1];
}
