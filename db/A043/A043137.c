/*
 * A043137  Numbers k such that 1 and 2 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_adj12
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043137:
   a(n) = the nth integer k (k ≥ 1) such that the base-7 representation
   of k contains the adjacent digits 1 and 2 in either order,
   while the base-7 representation of k‑1 does not. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adj12.h"

seq_t a(idx_t n)
{
    /* Static cache for already‑generated terms. */
    static seq_t *cache = NULL;      /* array storing the terms */
    static size_t cap   = 0;         /* capacity of cache */
    static size_t len   = 0;         /* number of terms stored */
    static idx_t  last  = 0;         /* last integer checked */

    /* Allocate initial cache if necessary. */
    if (cache == NULL)
    {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;  /* allocation failure – unlikely in tests */
    }

    /* Generate terms until we have at least n of them. */
    while (len < n)
    {
        last++;                         /* next integer to test */
        seq_t k = (seq_t)last;

        if (has_adj12(k) && !has_adj12(k - ((seq_t)1)))
        {
            /* Ensure sufficient capacity. */
            if (len == cap)
            {
                size_t newcap = cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (tmp == NULL)
                    return (seq_t)0;  /* realloc failure */
                cache = tmp;
                cap   = newcap;
            }
            cache[len++] = k;
        }
    }

    return cache[n - 1];
}
