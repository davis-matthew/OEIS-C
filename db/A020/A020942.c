/*
 * A020942  First column of 3rd-order Zeckendorf array A136189.
 * https://oeis.org/A020942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 8615, u32 n <= 8615, u64 n <= 8615,
 *            u128 n <= 8615, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A020942: first column of the 3rd‑order Zeckendorf array.
   The sequence consists of all integers n whose unique representation
   as a sum of distinct terms from
   {1, 2, 3, 4, 6, 9, 13, 19, …} (the sequence defined by
     t(1)=1, t(2)=2, t(3)=3, t(n)=t(n-1)+t(n-3) for n≥4)
   contains the term 1.  The representation has the rule that no two
   chosen indices are adjacent or pen‑adjacent (their difference is ≥3). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* helper: enlarge a 1‑based array*/
static void ensure_cap(seq_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t new_cap = *cap ? (*cap << 1) : 8;
    while (new_cap < need) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, (new_cap + 1) * sizeof(seq_t));
    *cap = new_cap;
}

/* 1‑based array of the allowed terms: 1,2,3,4,6,9,13,19,… */
static seq_t *term_arr = NULL;
static size_t term_len = 0;   /* last valid index */
static size_t term_cap = 0;

/* cache of the sequence values a(n) */
static seq_t *a_cache = NULL;
static size_t a_cache_len = 0;   /* number of stored terms */
static size_t a_cache_cap = 0;

/* current value to test; starts with 1 */
static seq_t next_candidate = 1;

/* extend term_arr until term_arr[term_len] > m (or initialise) */
static void extend_terms_up_to(seq_t m)
{
    if (term_len == 0) {
        ensure_cap(&term_arr, &term_cap, 3);
        term_arr[1] = (seq_t)1;
        term_arr[2] = (seq_t)2;
        term_arr[3] = (seq_t)3;
        term_len       = 3;
    }
    while (term_arr[term_len] <= m) {
        /* next = term[n-1] + term[n-3]  (n = term_len+1) */
        seq_t next = term_arr[term_len] + term_arr[term_len-2];
        ensure_cap(&term_arr, &term_cap, term_len + 1);
        ++term_len;
        term_arr[term_len] = next;
    }
}

/* return true iff the unique representation of m (with gap ≥3)
   contains term with index 1.  Greedy selection is correct. */
static bool has_repr_with_1(seq_t m)
{
    seq_t rem = m;
    /* prev is the last chosen index; larger than len means no restriction */
    size_t prev = term_len + 10;
    while (rem > 0) {
        size_t max_allowed;
        if (prev <= term_len) {
            if (prev <= 3) return false;      /* cannot pick any more */
            max_allowed = prev - 3;
        } else {
            max_allowed = term_len;
        }
        size_t idx = max_allowed;
        while (idx > 0 && term_arr[idx] > rem) {
            --idx;
        }
        if (idx == 0) return false;
        rem -= term_arr[idx];
        if (idx == 1) {
            return rem == 0;                   /* 1 must be the last term */
        }
        prev = idx;
    }
    return false;  /* unreachable */
}

/* public function required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* component offset is 1 */

    /* return from cache if already computed */
    if (a_cache_len >= (size_t)n) {
        return a_cache[(size_t)(n - 1)];
    }

    /* grow cache as needed */
    ensure_cap(&a_cache, &a_cache_cap, n);

    /* keep searching until we have n terms */
    while (a_cache_len < (size_t)n) {
        extend_terms_up_to(next_candidate);
        if (has_repr_with_1(next_candidate)) {
            a_cache[a_cache_len++] = next_candidate;
        }
        ++next_candidate;
    }
    return a_cache[(size_t)(n - 1)];
}
