/*
 * A014259  Iccanobif numbers: add reversal of a(n-1) to a(n-2).
 * https://oeis.org/A014259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-2) + rev_decimal(a(n-1)), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 39, u64 n <= 69, u128 n <= 132,
 *            big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* Ensure we can store at least indices 0 and 1 */
    if (cap < 2) {
        /* Grow to capacity of 2 (or larger if needed) */
        cap = 2;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* No negative indices allowed, they are not used in the problem */
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* Grow cache if necessary */
    if ((idx_t)cap <= n) {
        size_t new_cap = cap * 2;
        if (new_cap <= (size_t)n + 1) new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute values up to index n */
    for (idx_t i = 2; i <= n; ++i) {
        cache[i] = cache[i - 2] + reverse_decimal(cache[i - 1]);
    }

    return cache[n];
}
