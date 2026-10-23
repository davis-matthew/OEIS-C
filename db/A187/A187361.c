/*
 * A187361  Pell trisection: Pell(3*n+1), n >= 0.
 * https://oeis.org/A187361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) + a(n-2), a(0) = 1, a(1) = 12
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the (n‑th) Pell trisection term: Pell(3*n+1) */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised terms */
    static size_t len   = 0;      /* number of computed elements */
    static size_t cap   = 0;      /* allocated capacity */

    /* Initialise on first call */
    if (cap == 0) {
        cap   = 2;                      /* at least space for a(0) and a(1) */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;           /* no memory – return zero */
    }

    /* Grow the cache if necessary */
    while (cap <= (size_t)n) {
        size_t new_cap = cap * 2;
        if (cap == 0) new_cap = 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute new terms up to n */
    if (n + 1 > len) {
        for (size_t i = len; i <= (size_t)n; ++i) {
            if (i == 0)            cache[0] = 1;
            else if (i == 1)       cache[1] = 12;
            else                   cache[i] = 14 * cache[i - 1] + cache[i - 2];
        }
        len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
