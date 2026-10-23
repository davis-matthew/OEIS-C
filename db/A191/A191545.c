/*
 * A191545  Dispersion of (floor(9*n/4)), by antidiagonals.
 * https://oeis.org/A191545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   in_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A191545: Dispersion of floor(9*n/4) written along anti‑diagonals.
   For an index n≥1:
       * Find the unique k with k*(k+1)/2 ≥ n
         (k is the anti‑diagonal number).
       * Let i = n - (k-1)*k/2   (row number, 1‑based)
         and j = k+1-i             (column number, 1‑based).
       * Let s(x) = ⌊9x/4⌋.
       * Let t be the increasing complement of s
         (i.e. the set of positive integers not in {s(1),s(2),…}).
         Then a(n) = s^{\,j-1}(t(i)).
   The implementation below builds t incrementally and computes the
   required iterates of s.  No external libraries are used beyond the
   standard headers.  All arithmetic is performed with types that are
   guaranteed to be large enough for the intermediate results. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/in_s.h"

/* ------------------------------------------------------------------ */
/* Helper: test whether m belongs to the sequence s(n)=⌊9n/4⌋          */

/* ------------------------------------------------------------------ */
/* Cache for the complement sequence t: t[0] = t(1), t[1] = t(2), etc.*/
/* ------------------------------------------------------------------ */
static uint64_t *t_cache      = NULL;
static size_t   t_cache_size  = 0;
static size_t   t_cache_cap   = 0;
static uint64_t t_next_candidate = 1;   /* smallest integer not yet tested */

/* ------------------------------------------------------------------ */
/* Ensure that the cache contains at least 'needed' elements.         */
/* ------------------------------------------------------------------ */
static void ensure_t_cache(size_t needed)
{
    while (t_cache_size < needed) {
        if (t_cache_size >= t_cache_cap) {
            size_t new_cap = t_cache_cap ? t_cache_cap * 2 : 64;
            uint64_t *newp = (uint64_t *)realloc(t_cache,
                                                 new_cap * sizeof(uint64_t));
            if (!newp) { /* allocation failure – abort */
                abort();
            }
            t_cache = newp;
            t_cache_cap = new_cap;
        }
        /* Find the next integer not in s.  t_next_candidate points to the */
        /* first integer that has not yet been placed in the complement.   */
        while (in_s(t_next_candidate)) {
            ++t_next_candidate;
        }
        t_cache[t_cache_size++] = t_next_candidate;
        ++t_next_candidate;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* the sequence is 1‑based          */

    /* Find the anti‑diagonal number k so that (k-1)*k/2 < n ≤ k*(k+1)/2. */
    idx_t k = 1;
    while ((((unsigned __int128)k * (k + 1)) / 2) < n) {
        ++k;
    }

    /* Position inside the k‑th diagonal. */
    idx_t prev    = (idx_t)(((unsigned __int128)(k - 1) * k) / 2);
    idx_t offset  = n - prev;                     /* 1‑based in the diagonal  */
    idx_t i       = offset;                       /* row number, 1‑based      */
    idx_t j       = k + 1 - offset;               /* column number, 1‑based   */
    idx_t steps   = j - 1;                         /* iterates of s            */

    /* Ensure the complement t contains t(i). */
    ensure_t_cache((size_t)i);

    seq_t cur = (seq_t)t_cache[i - 1];            /* t(i)                     */
    for (idx_t s = 0; s < steps; ++s) {
        cur = (seq_t)(((seq_t)9 * cur) / 4);      /* apply s(x)=⌊9x/4⌋          */
    }
    return cur;
}
