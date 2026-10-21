/*
 * A215666  a(n) = 3*a(n-2) - a(n-3), with a(0)=0, a(1)=-3, and a(2)=6.
 * https://oeis.org/A215666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-2) - a(n-3), a(0)=0, a(1)=-3, a(2)=6
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 32, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_CACHE 10001   /* supports indices 0..10000 */

static seq_t cache[MAX_CACHE];
static idx_t cache_filled = 0;   /* number of elements that have been computed */

/* Return the n-th term of the sequence.  The table is filled on demand
   up to the required index, allowing calls in any order.  The routine
   is O(n) overall and uses static storage, avoiding the pitfalls of
   realloc on non‑trivial types. */
seq_t a(idx_t n)
{
    if (n >= MAX_CACHE) {
        /* n exceeds the pre‑allocated table.  For the intended use
           (n ≤ 10000) this should never happen. */
        return 0;
    }

    if (n < cache_filled)
        return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = cache_filled; i <= n; ++i) {
        if (i == 0)
            cache[i] = 0;
        else if (i == 1)
            cache[i] = -3;
        else if (i == 2)
            cache[i] = 6;
        else
            cache[i] = 3 * cache[i - 2] - cache[i - 3];
    }
    cache_filled = n + 1;
    return cache[n];
}
