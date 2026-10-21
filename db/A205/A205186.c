/*
 * A205186  Number of (n+1) X (n+1) 0..1 arrays with the number of clockwise
 *          edge increases in every 2 X 2 subblock differing from each
 *          horizontal or vertical neighbor.
 * https://oeis.org/A205186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-2) - 16*a(n-4) for n > 3 Initial values: a(1)=16,
 *            a(2)=40, a(3)=124, a(4)=256 (OEIS sequence A205186)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return the first four terms directly. */
    if (n <= 4) {
        switch ((idx_t)n) {
            case 1: return (seq_t)16;
            case 2: return (seq_t)40;
            case 3: return (seq_t)124;
            case 4: return (seq_t)256;
        }
    }

    /* Static cache for computed values (1‑indexed). */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;

    /* First allocation or resizing when necessary. */
    if (cache == NULL) {
        /* Allocate indices 0 .. n (index 0 unused). */
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed – unlikely */

        /* Initialise known base terms. */
        cache[1] = (seq_t)16;
        cache[2] = (seq_t)40;
        cache[3] = (seq_t)124;
        cache[4] = (seq_t)256;
        max_n   = 4;
    } else if (n > max_n) {
        /* Grow the array to accommodate up to n. */
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* reallocation failed – unlikely */
        cache = tmp;
    }

    /* Compute any missing terms up to n using the recurrence. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* i-2 and i-4 are already computed (i >= 5 here). */
        cache[i] = (cache[i - 2] * (seq_t)8) - (cache[i - 4] * (seq_t)16);
    }
    max_n = n;   /* update maximum computed index */

    return cache[n];
}
