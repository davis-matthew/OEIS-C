/*
 * A137230  Composite numbers that are divisible by the number of their prime
 *          factors (counted with multiplicity).
 * https://oeis.org/A137230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the list of composite integers that are divisible by
   the number of their prime factors counted with multiplicity
   (OEIS A137230).  The nth term is returned for 1 ≤ n. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x) */
#include "../utils/bigomega.h"   /* seq_t bigomega(idx_t n) */

/* static storage for memoization ------------------------------------ */
static seq_t *term_cache = NULL;   /* array of cached terms */
static size_t term_cap   = 0;      /* capacity of the array */
static size_t term_used  = 0;      /* number of terms currently stored */
static idx_t  next_cand  = 4;      /* next integer to test after cache */

/* Ensure that term_cache has at least 'needed' free slots. */
static void ensure_capacity(size_t needed)
{
    if (needed <= term_cap)
        return;
    size_t new_cap = term_cap ? term_cap * 2 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    term_cache = (seq_t *)realloc(term_cache, new_cap * sizeof(seq_t));
    term_cap   = new_cap;
}

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)                       /* undefined for n < 1 */
        return 0;

    /* Grow the cache until it holds at least n terms */
    while (term_used < (size_t)n) {
        /* Test the current candidate number for the property */
        if (!is_prime(next_cand)) {                     /* composite */
            seq_t omega = bigomega((idx_t)next_cand);    /* Ω(n) */
            if (omega != 0) {                           /* safety check */
                seq_t val = (seq_t)next_cand;
                if (val % omega == 0) {                 /* property holds */
                    ensure_capacity(term_used + 1);
                    term_cache[term_used++] = val;
                }
            }
        }
        ++next_cand;
    }

    return term_cache[n - 1];
}
