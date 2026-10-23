/*
 * A118152  Start with 1 and repeatedly reverse the digits and add 56 to get
 *          the next term.
 * https://oeis.org/A118152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n) = rev(a(n-1)) + 56 for n ≥ 2
   (where rev(x) is the decimal reversal of x) */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    /* undefined behavior for n < 1; the caller should never use it */
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache = NULL;      /* array of computed terms 1‑based    */
    static idx_t last_computed = 0;  /* index of the last term stored      */
    static size_t cap = 0;           /* allocated capacity of cache        */

    /* Ensure capacity for the requested index */
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0)
            abort();               /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* Initialize base term if necessary */
    if (last_computed == 0) {
        cache[0] = (seq_t)1;        /* a(1) */
        last_computed = 1;
    }

    /* Compute terms up to n (ignoring any already computed) */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i - 1] = reverse_decimal(cache[i - 2]) + (seq_t)56;
    }

    last_computed = n;
    return cache[n - 1];
}
