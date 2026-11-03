/*
 * A044257  Numbers n such that string 0,6 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044257
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

/* Sequence: Numbers n such that the base‑9 representation of n contains
   the substring "06" while the base‑9 representation of n-1 does not.
   The list starts with a(1)=87.  This implementation performs a
   linear search with memoisation. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff the base‑9 representation of x contains the
   substring "06" (i.e. a 6 followed by a 0 in the next higher digit).
   Leading zeros are not considered; therefore for a single‑digit 6
   the substring is not found. */
static inline bool has_sub06(seq_t x)
{
    if (x == 0) return false;

    seq_t y = x;
    while (y > 0)
    {
        seq_t d = y % 9;     /* current least‑significant digit   */
        y      = y / 9;      /* move to next more significant digit */

        /* Look for pattern 0 6: current digit is 6 and the
           next (higher) digit exists and is 0. */
        if (d == 6 && y > 0 && (y % 9) == 0)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, but harmless for n<1 */

    /* Static cache of already found terms. */
    static seq_t *terms          = NULL;
    static size_t terms_len      = 0;
    static size_t terms_cap      = 0;
    static seq_t next_candidate  = 1;   /* next integer to test     */

    /* Return cached term if it exists. */
    if (n <= terms_len)
        return terms[n - 1];

    /* Initialise cache on first call. */
    if (terms_len == 0)
    {
        terms_cap = 16;
        terms = mem_new(seq_t, terms_cap);
        if (!terms) return 0;    /* allocation failure      */
    }

    seq_t x       = next_candidate;
    size_t found  = terms_len;

    /* Search until we have found the n‑th term. */
    while (found < n)
    {
        if (has_sub06(x) && !has_sub06(x - 1))
        {
            /* Grow the cache if needed. */
            if (terms_len == terms_cap)
            {
                terms_cap *= 2;
                seq_t *tmp = mem_resize(seq_t, terms, terms_cap);
                if (!tmp) return 0;   /* allocation failure      */
                terms = tmp;
            }
            terms[terms_len++] = x;
            ++found;
        }
        ++x;
    }

    /* Update next_candidate for subsequent calls. */
    next_candidate = x;

    return terms[n - 1];
}
