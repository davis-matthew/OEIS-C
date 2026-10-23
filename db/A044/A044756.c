/*
 * A044756  Numbers n such that string 4,3 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044756: numbers n such that the decimal representation of n contains
   the substring "43" but the decimal representation of n+1 does not. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Test whether x contains the digit pair 4 followed by 3 in its decimal
   representation.  Works for any seq_t type that supports % and / 10. */
static bool has_43(seq_t x)
{
    seq_t prev = 10;                     /* sentinel outside 0‑9 */
    while (x > 0) {
        seq_t d = x % 10;
        if (prev == 3 && d == 4) return true;
        prev = d;
        x = x / 10;
    }
    return false;
}

/* Return the n‑th term of A044756 (n ≥ 1).  The result is memoised so
   that calls in any order are fast. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* sequence starts at index 1 */

    static seq_t *cache = NULL;        /* storage for found terms          */
    static size_t len  = 0;            /* number of terms stored            */
    static size_t cap  = 0;            /* current capacity of cache         */

    /* Initialise storage on first call.  Allocate a modest start size. */
    if (cap == 0) {
        cap = 16384;                   /* should accommodate n up to ~10⁴ */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */
    }

    /* If we already have the requested term, return it. */
    if (len >= (size_t)n)
        return cache[(size_t)n - 1];

    /* Candidate to test is one more than the last stored term,
       or 1 if none have been stored yet. */
    seq_t candidate = (len > 0) ? cache[len - 1] + 1 : 1;

    /* Generate terms until we reach the desired index. */
    while (len < (size_t)n) {
        /* Find the next number satisfying the condition. */
        while (!(has_43(candidate) && !has_43(candidate + 1)))
            candidate = candidate + 1;

        /* Grow storage if necessary. */
        if (len == cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;        /* allocation failure */
            cache = tmp;
            cap   = new_cap;
        }

        /* Store the found term. */
        cache[len++] = candidate;

        /* Next candidate must be larger than the one just stored. */
        candidate = candidate + 1;
    }

    return cache[(size_t)n - 1];
}
