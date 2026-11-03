/*
 * A044448  Numbers n such that string 1,2 occurs in the base 4 representation
 *          of n but not of n+1.
 * https://oeis.org/A044448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044448:
 * Numbers n such that the base‑4 representation of n contains the consecutive
 * digits '1,2' but the base‑4 representation of n+1 does not.
 * The function caches results to handle non‑sequential queries.
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include "../utils/mem.h"

static inline int has12(seq_t x)
{
    /* Return true if base‑4 digits of x contain the pattern 1,2 (1 higher, 2 lower). */
    seq_t prev = 4;                 /* sentinel (not a valid base‑4 digit) */
    while (x > 0)
    {
        seq_t d = x % 4;
        if (prev == 2 && d == 1)   /* found 1 followed by 2 (higher‑order digit 1) */
            return 1;
        prev = d;
        x /= 4;
    }
    return 0;
}

static inline int qualifies(seq_t n)
{
    return has12(n) && !has12(n + 1);
}

seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined for non‑positive indices */
        return 0;

    /* ---------- static cache of found terms ---------- */
    static seq_t *cache = NULL;     /* array of found values */
    static size_t cap   = 0;        /* current allocation size */
    static size_t size  = 0;        /* number of elements stored */
    static seq_t next   = 1;        /* next integer to test */

    /* Ensure cache can hold at least n elements. */
    if (size < n && cap < n)
    {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp)
            abort();                  /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Scan integers until we have found at least n values. */
    while (size < n)
    {
        if (qualifies(next))
        {
            /* store the found value */
            cache[size++] = next;
        }
        ++next;
    }

    return cache[n - 1];
}
