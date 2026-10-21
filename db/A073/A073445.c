/*
 * A073445  Second differences of A002808, the sequence of composites.
 * https://oeis.org/A073445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = c(n+2) - 2*c(n+1) + c(n) where c(n) is the n-th composite
 *            number (A002808), with c(1)=4, c(2)=6, …
 * Range:     i8 n <= 96, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: In C++ the cast of the result of realloc/malloc to seq_t*
   is a C-style cast. This works for both plain integer types
   and for arbitrary‑precision class types. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeTrial.h" /* bool isPrimeTrial(seq_t n) */

static seq_t *composite_list = NULL;   /* array of composites, 0‑based: composite_list[0] = 4 */
static size_t comp_cnt   = 0;          /* number of composites stored */
static size_t comp_cap   = 0;          /* current capacity of composite_list */
static idx_t  last_checked  = 3;       /* last integer examined for compositeness */

/* Ensure composite_list contains at least m composites (m ≥ 1). */
static void ensure_composites(idx_t m)
{
    if (comp_cnt >= (size_t)m) return;

    while (comp_cnt < (size_t)m) {
        ++last_checked;
        /* Test whether last_checked is composite. */
        if (!isPrimeTrial((seq_t)last_checked)) {
            /* Grow the array if necessary. */
            if (comp_cnt >= comp_cap) {
                size_t new_cap = comp_cap ? (comp_cap << 1) : 16;
                seq_t *tmp = (seq_t *)realloc(composite_list,
                                              new_cap * sizeof(seq_t));
                if (!tmp) abort();            /* out of memory – terminate */
                composite_list = tmp;
                comp_cap = new_cap;
            }
            composite_list[comp_cnt++] = (seq_t)last_checked;
        }
    }
}

/* Public function: a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1, return 0 as safe default */

    /* Ensure we have the composites up to index n+2. */
    ensure_composites(n + 2);

    /* Retrieve c(n), c(n+1), c(n+2).  composite_list is 0‑based. */
    seq_t c_n   = composite_list[(size_t)n - 1];
    seq_t c_np1 = composite_list[(size_t)n];
    seq_t c_np2 = composite_list[(size_t)n + 1];

    /* Compute second difference: c(n+2) - 2*c(n+1) + c(n). */
    return c_np2 - (c_np1 + c_np1) + c_n;
}
