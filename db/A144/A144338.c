/*
 * A144338  Squarefree numbers > 1.
 * https://oeis.org/A144338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nth squarefree integer > 1 (computed by incremental
 *            check)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cached squarefree numbers */
    static idx_t   count = 0;       /* how many have been cached */
    static size_t  cap   = 0;       /* capacity of cache */
    static idx_t   last_checked = 1;/* last integer examined for squarefreeness */

    /* Extend cache until we have at least n terms */
    while (count < n) {
        if (count == cap) {
            size_t new_cap = (cap == 0) ? 16 : cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();     /* allocation failed */
            cache = tmp;
            cap = new_cap;
        }
        ++last_checked;
        if (is_squarefree((seq_t)last_checked)) {
            cache[count++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
