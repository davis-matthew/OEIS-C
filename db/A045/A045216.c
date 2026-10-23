/*
 * A045216  Numbers whose base-5 representation contains exactly three 0's and
 *          two 4's.
 * https://oeis.org/A045216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers in base‑5 with exactly three 0’s and two 4’s
 * Range:     u8 none, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   has_prop
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_prop.h"

/* Return the n‑th term (n ≥ 1) of the sequence. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t sz    = 0;
    static seq_t next   = 1;      /* next integer to test */

    if (n == 0)               /* offset is 1; guard against 0 */
        return 0;

    /* Generate terms until we have at least n stored. */
    if ((idx_t)sz < n) {
        while ((idx_t)sz < n) {
            /* Find the next integer satisfying the property. */
            while (!has_prop(next))
                next++;

            /* Ensure the cache has room for the new term. */
            if (sz >= cap) {
                size_t newcap = (cap == 0) ? 16 : cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp)          /* allocation failure – return 0 */
                    return 0;
                cache = tmp;
                cap   = newcap;
            }
            cache[sz++] = next;
            next++;              /* advance to the next candidate */
        }
    }
    return cache[n - 1];
}
