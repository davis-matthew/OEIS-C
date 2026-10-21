/*
 * A221756  Number of 2 X n arrays of occupancy after each element moves to
 *          some horizontal or antidiagonal neighbor, without move-in move-out
 *          straight through or left turns.
 * https://oeis.org/A221756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-2) + a(n-3) - 4*a(n-4), * recurrence
 *            initial values: a(1)=0, a(2)=2, a(3)=1, a(4)=9
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ensure_capacity.h"

static seq_t *cache = NULL;
static size_t  cap   = 0;
static size_t  len   = 0;

seq_t a(idx_t n)
{
    /* Initialise the base four terms once */
    if (len == 0) {
        /* guarantee space for at least the first four values */
        ensure_capacity(4, &cap, (void **)&cache, sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)9;
        len = 4;
    }

    /* If the requested value is already computed, return it */
    if (n <= len)
        return cache[n - 1];

    /* Expand cache to hold up to n values */
    ensure_capacity((size_t)n, &cap, (void **)&cache, sizeof(seq_t));

    /* Compute terms using the recurrence from len up to n-1 (0‑based) */
    for (idx_t i = len; i < n; ++i) {
        seq_t term = cache[i - 1]                      /* a(n-1) */
                  + (cache[i - 2] * (seq_t)6)          /* + 6*a(n-2) */
                  + cache[i - 3]                      /* + a(n-3) */
                  - (cache[i - 4] * (seq_t)4);        /* - 4*a(n-4) */
        cache[i] = term;
    }

    len = (size_t)n;
    return cache[n - 1];
}
