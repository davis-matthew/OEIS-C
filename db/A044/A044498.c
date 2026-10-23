/*
 * A044498  Numbers n such that string 2,3 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   has23_in_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n): numbers n such that the base‑6 representation of n contains
   the substring "23" but the base‑6 representation of n+1 does not.
   We generate the terms on demand, keeping all found values in a static
   cache so that calls can come in any order. */

/* ---------------- standard includes ---------------- */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has23_in_base6.h"

/* --------- helper: does the number satisfy the definition? */
static bool satisfies(seq_t n)
{
    return has23_in_base6(n) && !has23_in_base6(n + 1);
}

/* --------- memoization infrastructure ---------------- */
static seq_t *cache = NULL;              /* stored terms */
static idx_t  cache_len = 0;             /* number of stored terms */
static idx_t  cache_cap = 0;             /* allocated capacity */
static idx_t  next_candidate = 1;        /* next integer to examine */

/* --------- grow cache if needed ----------------------- */
static void ensure_capacity(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        free(cache);
        abort(); /* unrecoverable – used only for test harnesses */
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

/* -------------------- main function -------------------- */
seq_t a(idx_t n)
{
    if (n == 0)   /* the sequence is 1‑indexed; return 0 for safety */
        return 0;

    /* keep generating terms until we have at least n of them */
    while (cache_len < n) {
        /* find the next integer that satisfies the condition */
        while (true) {
            seq_t cand = (seq_t)next_candidate;
            if (satisfies(cand)) {
                /* store the found term */
                ensure_capacity(cache_len + 1);
                cache[cache_len++] = cand;
                next_candidate++;           /* advance past this number */
                if (cache_len == n)         /* the n-th term found */
                    return cand;
                break;                      /* continue searching */
            }
            ++next_candidate;               /* try the next number */
        }
    }
    /* in the (unlikely) event that cache_len >= n is already true */
    return cache[n - 1];
}
