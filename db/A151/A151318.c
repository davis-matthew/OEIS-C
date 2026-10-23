/*
 * A151318  Number of walks within N^2 (the first quadrant of Z^2) starting at
 *          (0,0) and consisting of n steps taken from {(-1, 1), (-1, 0), (0,
 *          1), (1, 0), (1, 1)}.
 * https://oeis.org/A151318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, changed
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: (n + 1) * a(n) = 2 * (n + 2) * a(n‑1) + 15 * (n – 1) * a(n‑2)
   Initial values: a(0) = 1, a(1) = 3 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Compute the nth term of OEIS A151318. */
seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Static memoisation cache. */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;   /* Allocated size of cache. */
    size_t          old_cap = cap;

    /* Ensure the cache is large enough to hold indices up to n. */
    if (n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)   /* Out of memory: return 0 to signal failure. */
            return (seq_t)0;
        /* Initialise any newly allocated part with zero. */
        if (new_cap > cap) {
            memset(tmp + cap, 0, (new_cap - cap) * sizeof(seq_t));
        }
        cache = tmp;
        cap   = new_cap;
        /* Set the base values (they may be overwritten but are harmless). */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
    }

    /* Compute any terms that have not yet been calculated.  old_cap may be
       less than 2, so we start from 2. */
    size_t start = old_cap;
    if (start < 2) start = 2;
    for (size_t i = start; i <= (size_t)n; ++i) {
        /* (n + 1) * a(n) = 2 * (n + 2) * a(n-1) + 15 * (n - 1) * a(n-2) */
        seq_t term1 = ((seq_t)(i + 2)) * cache[i - 1];
        term1 *= 2;
        seq_t term2 = ((seq_t)(i - 1)) * cache[i - 2];
        term2 *= 15;
        seq_t num   = term1 + term2;
        seq_t denom = (seq_t)(i + 1);
        cache[i] = num / denom;   /* Exact division. */
    }

    return cache[n];
}
