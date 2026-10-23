/*
 * A043184  Numbers k such that 0 and 5 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_adjacent_05
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A043184: Numbers k such that 0 and 5 occur juxtaposed in the base‑9
 * representation of k but not of k‑1.
 * The function computes the nth term (1‑based) by scanning integers
 * incrementally, storing the results in a static cache so that
 * successive calls are fast.
 * Complexity: roughly O(n * log₉(max(k))) < 2 s for n ≤ 10 000.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent_05.h"

/* Dynamic cache for the sequence. */
static seq_t *cache   = NULL;
static size_t  cap    = 0;
static size_t  used   = 0;     /* number of terms stored */
static idx_t   next_k = 1;     /* next integer to test for candidacy */

/* Ensure the cache has capacity for `need` elements. */
static void ensure_capacity(size_t need)
{
    if (need <= cap)
        return;
    size_t new_cap = (cap == 0) ? 4 : cap * 2;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort(); /* handle allocation failure */
    cache = new_ptr;
    cap   = new_cap;
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; not used */
        return 0;

    /* Grow the cache until it contains at least n terms. */
    while (used < n) {
        idx_t k = next_k++;
        if (!has_adjacent_05(k))
            continue;  /* k itself must contain 0 and 5 adjacent */
        if (has_adjacent_05(k - 1))
            continue;  /* k‑1 must NOT contain them */

        /* Store the new term. */
        ensure_capacity(used + 1);
        cache[used++] = (seq_t)k;
    }

    return cache[n - 1];
}
