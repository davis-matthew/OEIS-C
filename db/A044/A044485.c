/*
 * A044485  Numbers n such that string 0,2 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_0_2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044485:
   numbers n such that the base‑6 representation of n contains the
   substring "0,2" but the representation of n+1 does not.
   The implementation builds the sequence incrementally by scanning
   integers and caching all qualifying terms.  Each call works for
   any previously requested index regardless of call order. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has_0_2.h"

/* forward declarations */
static bool has_0_2(seq_t x);
static bool is_target(seq_t n);

/* ------------------------------------------------------------------ */
/* Helpers to detect the pattern 0,2 in the base‑6 representation     */

static bool is_target(seq_t n)
{
    return has_0_2(n) && !has_0_2(n + (seq_t)1);
}

/* ------------------------------------------------------------------ */
/* Cache for already-found sequence terms                             */
/* ------------------------------------------------------------------ */
static seq_t *cache        = NULL;   /* array holding qualifying terms   */
static size_t cache_len    = 0;      /* number of terms stored           */
static size_t cache_cap    = 0;      /* current capacity of the array    */
static seq_t next_candidate = 0;     /* next integer to examine         */

/* Ensure that the first `needed` terms are in the cache.           */
static void ensure_cache(idx_t needed)
{
    /* Allocate an initial capacity if the cache is empty.          */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < (size_t)needed) {
        /* If the candidate satisfies the property, store it.     */
        if (is_target(next_candidate)) {
            /* Grow the array if required.                         */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        /* Move on to the next integer.                             */
        next_candidate++;
    }
}

/* ------------------------------------------------------------------ */
/* OEIS sequence accessor                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1; a(0) is undefined */
        return 0;
    if ((size_t)n > cache_len)
        ensure_cache(n);
    return cache[n - 1];
}
