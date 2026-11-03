/*
 * A044689  Numbers n such that string 6,3 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A044689: the n‑th integer whose base‑9 representation contains the
   substring 63 but whose successor does not.  Computed by a linear
   search with on‑demand caching.  Works for all seq_t and idx_t
   types that support the arithmetic operations below. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Returns true if the base‑9 representation of x contains "63". */
static bool has_63(seq_t x)
{
    unsigned char digits[64];
    size_t len = 0;

    /* Decompose into base‑9 digits, least significant first. */
    do {
        digits[len++] = (unsigned char)(x % 9);
        x /= 9;
    } while (x != 0);

    /* Scan for 6 followed by 3 in the most‑significant‑to‑least order. */
    while (len-- > 1)
        if (digits[len] == 6 && digits[len - 1] == 3)
            return true;

    return false;
}

/* Return the n‑th term of A044689 (offset 1).  Any ordering of calls
   is supported; the result is memoized for efficiency. */
seq_t a(idx_t n)
{
    if (n == 0)                /* behaviour for n < 1 is irrelevant */
        return (seq_t)0;

    static seq_t *cache = NULL;   /* cached terms */
    static size_t cap = 0;        /* capacity of cache */
    static idx_t  found = 0;      /* number of terms currently cached */
    static seq_t last_checked = (seq_t)0;   /* next integer to test */

    /* Extend cache until the n‑th term is found. */
    while (found < n)
    {
        /* Test current candidate. */
        if (has_63(last_checked) && !has_63(last_checked + (seq_t)1))
        {
            /* Grow cache if needed. */
            if (found == cap)
            {
                size_t newCap = cap ? cap << 1 : 4;
                seq_t *tmp = mem_resize(seq_t, cache, newCap);
                if (!tmp) /* allocation failed – return 0 as a safe fallback. */
                    return (seq_t)0;
                cache = tmp;
                cap = newCap;
            }
            cache[found++] = last_checked;   /* store new term. */
        }
        ++last_checked;  /* advance to next integer. */
    }

    return cache[n - 1];    /* 1‑based indexing. */
}
