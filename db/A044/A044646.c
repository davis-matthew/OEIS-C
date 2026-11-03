/*
 * A044646  Numbers k such that string 1,5 occurs in the base 9 representation
 *          of k but not of k+1.
 * https://oeis.org/A044646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that the base‑9 representation of k contains
 * the substring "15" but the base‑9 representation of k+1 does not.
 * Computed by a simple incremental search with memoisation. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Return true if x (treated as uint64_t) has an adjacent pair of base‑9
 * digits 1 followed by 5 (most significant to least). */
static inline bool contains15_idx(idx_t x)
{
    int prev = -1;                 /* previous less‑significant digit */
    while (x > 0)
    {
        int cur = (int)(x % 9);     /* least‑significant base‑9 digit */
        x = x / 9;
        if (prev != -1 && cur == 1 && prev == 5)
            return true;
        prev = cur;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t  *cache = NULL;   /* array of terms */
    static size_t  len   = 0;      /* number of cached terms */
    static size_t  cap   = 0;      /* allocated capacity */
    static idx_t   cur_k = 0;      /* smallest k not yet examined */

    /* No behaviour defined for n==0; OEIS offset is 1. */
    if (n == 0)
        return 0;

    /* Grow cache until it holds at least n terms. */
    while (len < n)
    {
        cur_k = cur_k + 1;            /* next integer to test */

        if (contains15_idx(cur_k) &&
            !contains15_idx(cur_k + 1))
        {
            /* Ensure space for new element. */
            if (len == cap)
            {
                size_t newcap = (cap == 0) ? 16 : cap << 1;
                seq_t *tmp = mem_resize(seq_t, cache, newcap);
                /* The problem guarantees that memory allocation succeeds. */
                cache = tmp;
                cap   = newcap;
            }
            cache[len++] = (seq_t)cur_k;   /* store the term */
        }
    }

    return cache[n - 1];             /* OEIS indices start at 1 */
}
