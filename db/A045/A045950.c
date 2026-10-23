/*
 * A045950  Number of upward triangles in a Star of David matchstick
 *          arrangement of size n.
 * https://oeis.org/A045950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(10*n^2+9*n+1)/2 (via the 4-th order recurrence)
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 950, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External definition required by the tester */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* number of entries currently stored (>=1 if non‑NULL) */

    /* Ensure capacity for the requested index and compute missing values. */
    if (cache == NULL) {
        /* Allocate at least 4 entries for the base values. */
        cache_len = (n >= 3 ? n + 1 : 4);
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 10;
        cache[2] = 59;
        cache[3] = 177;
        /* The rest (if any) will be filled below. */
    }
    else if (n >= cache_len) {
        /* Grow the array. */
        cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        cache_len = n + 1;
    }

    /* Compute values that are not yet known, starting just after the last
       pre‑computed index.  The base values (indices 0‑3) are already present. */
    for (idx_t i = cache[0] ? 0 : 4; i <= n; ++i) { /* when cache_len>0, i starts at 4 */
        if (i < 4) continue;          /* base values already set */
        cache[i] = 4*cache[i-1] - 6*cache[i-2] + 4*cache[i-3] - cache[i-4];
    }

    return cache[n];
}
