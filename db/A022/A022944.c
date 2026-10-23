/*
 * A022944  a(n) = a(n-2) + c(n-1) for n >= 3, a( ) increasing, given a(1)=2,
 *          a(2)=4; where c( ) is complement of a( ).
 * https://oeis.org/A022944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + c(n-1) (n ≥ 3) Initial values: a(1) = 2, a(2) =
 *            4 where c(k) is the k‑th positive integer not in the sequence a
 * Range:     u8 n <= 26, u16 n <= 487, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* static cache for the sequence a */
static seq_t *a_cache          = NULL;
static size_t a_len            = 0;   /* number of elements currently cached */
static size_t a_cap            = 0;   /* capacity of a_cache */

/* static cache for the complement sequence c */
static seq_t *c_cache          = NULL;
static size_t c_len            = 0;   /* number of elements cached */
static size_t c_cap            = 0;   /* capacity of c_cache */

/* helpers for generating the complement */
static idx_t  comp_next_candidate = 1;   /* next integer to test for complement */
static idx_t  comp_scan_idx       = 0;   /* index into a_cache used during testing */

/* Grow the a_cache to accommodate at least newlen elements */
static void ensure_a_cap(size_t newlen)
{
    if (a_cap >= newlen) return;
    size_t cap = a_cap ? a_cap * 2 : 1;
    while (cap < newlen) cap *= 2;
    a_cache = (seq_t *)realloc(a_cache, cap * sizeof(seq_t));
    a_cap   = cap;
}

/* Grow the c_cache to accommodate at least newlen elements */
static void ensure_c_cap(size_t newlen)
{
    if (c_cap >= newlen) return;
    size_t cap = c_cap ? c_cap * 2 : 1;
    while (cap < newlen) cap *= 2;
    c_cache = (seq_t *)realloc(c_cache, cap * sizeof(seq_t));
    c_cap   = cap;
}

/* Ensure that c_cache contains the first 'need' values of the complement */
static void ensure_comp_len(idx_t need)
{
    /* 'need' is a 1‑based upper bound: we need indices 1..need inclusive,
       i.e. c_cache[0..need-1] must exist */
    while (c_len < (size_t)need) {
        /* Skip values that are in the already known part of a_cache */
        while (comp_scan_idx < a_len &&
               (idx_t)a_cache[comp_scan_idx] < comp_next_candidate)
            comp_scan_idx++;

        if (comp_scan_idx < a_len &&
            (idx_t)a_cache[comp_scan_idx] == comp_next_candidate) {
            comp_next_candidate++;               /* integer is in a, not a complement */
            continue;
        }

        /* The current candidate is a complement number */
        ensure_c_cap(c_len + 1);
        c_cache[c_len++] = (seq_t)comp_next_candidate;
        comp_next_candidate++;
    }
}

/* Main function required by the interface */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n==0, but safe return */

    /* Build the sequence up to index n if needed */
    while (a_len < (size_t)n) {
        idx_t idx = a_len + 1;      /* 1‑based index of the element to compute */
        seq_t val;

        if (idx == 1) {
            val = (seq_t)2;
        } else if (idx == 2) {
            val = (seq_t)4;
        } else {
            /* The recurrence needs c(idx-1) and a(idx-2) */
            ensure_comp_len(idx - 1);                /* c(idx-1) is required */
            val = a_cache[idx - 3] +                /* a(idx-2)           */
                  c_cache[idx - 2];                 /* c(idx-1)           */
        }

        ensure_a_cap(a_len + 1);
        a_cache[a_len++] = val;
    }

    return a_cache[n - 1];
}
