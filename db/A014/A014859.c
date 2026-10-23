/*
 * A014859  Numbers k that divide s(k), where s(1)=1, s(j)=12*s(j-1)+j.
 * https://oeis.org/A014859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A014859:  k such that k divides s(k), where
     s(1) = 1,  s(j) = 12 * s(j-1) + j.
   The function computes the terms on demand and memoises them
   so that calls may arrive in any order.  The algorithm
   keeps s(j) modulo every integer up to a fixed maximum.
   Operations are performed in uint32_t to avoid overflow
   (the modulus never exceeds the maximum).  For arbitrary‑precision
   builds the result type seq_t is used to store the found k values. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const size_t INIT_CAP = 20000;          /* assumed max n needed */

static seq_t   *results   = NULL;              /* memoised a(n) values   */
static size_t  results_len  = 0;
static size_t  results_cap  = 0;

static uint32_t *s_mod      = NULL;            /* s(j) modulo d arrays   */
static size_t   s_mod_cap   = 0;               /* current capacity      */

static idx_t    last_j      = 0;               /* last processed index  */
static bool     initialized = false;

/* Reinitialise s_mod array for a new capacity and recompute values
   up to currently processed last_j.  This is required only if a
   request exceeds the current capacity. */
static void extend_s_mod(size_t new_cap)
{
    uint32_t *tmp = (uint32_t *)realloc(s_mod, (new_cap + 1) * sizeof(uint32_t));
    if (!tmp) return;                   /* allocation failure ignored */
    s_mod      = tmp;
    s_mod_cap  = new_cap;

    /* initialise from s(1) = 1 */
    for (size_t d = 1; d <= s_mod_cap; ++d)
        s_mod[d] = 1 % d;

    /* rebuild all intermediate states up to last_j */
    for (idx_t j = 2; j <= last_j; ++j)
        for (size_t d = 1; d <= s_mod_cap; ++d)
            s_mod[d] = (12u * s_mod[d] + (uint32_t)j) % (uint32_t)d;
}

/* Append a new element to the results array, growing if needed. */
static void append_result(seq_t val)
{
    if (results_len == results_cap) {
        size_t new_cap = results_cap ? results_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
        if (!tmp) return;               /* allocation failure ignored */
        results = tmp;
        results_cap = new_cap;
    }
    results[results_len++] = val;
}

seq_t a(idx_t n)
{
    /* initialise static structures on first call */
    if (!initialized) {
        s_mod_cap = INIT_CAP;
        s_mod     = (uint32_t *)malloc((s_mod_cap + 1) * sizeof(uint32_t));
        for (size_t d = 1; d <= s_mod_cap; ++d)
            s_mod[d] = 1 % d;           /* s(1) = 1 */
        last_j = 1;
        /* j = 1 always satisfies k | s(k) because 1 | 1 */
        append_result(1);
        initialized = true;
    }

    /* Ensure capacity for the requested index */
    if (n > s_mod_cap) {
        /* extend to at least n, double for future growth */
        size_t new_cap = n * 2;
        if (new_cap < INIT_CAP) new_cap = INIT_CAP;
        extend_s_mod(new_cap);
    }

    /* Compute subsequent s(j) values until we have n terms */
    while (results_len < (size_t)n) {
        idx_t j = last_j + 1;           /* next index to process */

        /* Update all residues modulo d */
        for (size_t d = 1; d <= s_mod_cap; ++d)
            s_mod[d] = (12u * s_mod[d] + (uint32_t)j) % (uint32_t)d;

        /* If j divides its s(j), store j */
        if (s_mod[(size_t)j] == 0)
            append_result((seq_t)j);

        last_j = j;
    }

    return results[n - 1];
}
