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
 * Helpers:   has12
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044448:
 * Numbers n such that the base‑4 representation of n contains the consecutive
 * digits '1,2' but the base‑4 representation of n+1 does not.
 * The function caches results to handle non‑sequential queries.
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include "../utils/has12.h"

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
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
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
