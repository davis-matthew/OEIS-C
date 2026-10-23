/*
 * A118518  Define sequence S_m by: initial term = m, reverse digits and add 3
 *          to get next term. Entry shows S_2. This reaches a cycle of length
 *          6 in 3 steps.
 * https://oeis.org/A118518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 2, a(n) = rev(a(n-1)) + 3 (for n > 1)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* rev reverses the decimal digits of the previous term.       */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    /* static cache to avoid recomputation across calls     */
    static seq_t *cache = NULL;   /* array of computed terms */
    static size_t cap   = 0;      /* allocated capacity      */
    static size_t used  = 0;      /* number of terms stored  */

    /* ensure we have at least n terms in the cache           */
    if ((size_t)n > cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        while ((size_t)n > new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        /* if realloc fails, we simply abort (build fails)      */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms                              */
    if (used == 0) {                       /* first call has empty cache */
        cache[0] = (seq_t)2;                /* a(1) */
        used = 1;
    }
    while (used < (size_t)n) {
        /* next term = reverse(previous) + 3              */
        cache[used] = reverse_decimal(cache[used-1]) + (seq_t)3;
        used++;
    }

    return cache[n-1];
}
