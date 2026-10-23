/*
 * A039341  Numbers whose base-8 representation has the same nonzero number of
 *          1's and 7's.
 * https://oeis.org/A039341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   same_nonzero_ones_sevens
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A039341 – Numbers whose base‑8 representation has the same
   nonzero number of 1's and 7's.  This routine returns the n‑th such
   number (a(1)=15).  The implementation uses a simple search with
   memoisation; it works for any integer type seq_t (built‑in or an
   arbitrary‑precision class) and for any size of n. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/same_nonzero_ones_sevens.h"

/* Return the n‑th element of the sequence.  n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n == 0 */

    static seq_t *list = NULL;          /* stored elements */
    static size_t len   = 0;            /* number stored */
    static size_t cap   = 0;            /* allocated capacity */
    static seq_t cur = 1;               /* next candidate to test */

    /* Generate terms until we have at least n of them */
    while (len < (size_t)n)
    {
        if (same_nonzero_ones_sevens(cur))
        {
            /* Grow array if needed */
            if (len == cap)
            {
                size_t new_cap = (cap == 0) ? 16 : cap * 2;
#ifdef __cplusplus
                seq_t *tmp = new seq_t[new_cap];
                /* Copy existing elements */
                for (size_t i = 0; i < len; ++i)
                    tmp[i] = list[i];
                delete[] list;
#else
                seq_t *tmp = (seq_t *)realloc(list, new_cap * sizeof(seq_t));
                if (!tmp) abort();
#endif
                list = tmp;
                cap = new_cap;
            }
            list[len++] = cur;
        }
        ++cur;
    }
    return list[n - 1];
}
