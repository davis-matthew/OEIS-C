/*
 * A224547  Number of (n+1) X 6 0..1 matrices with each 2 X 2 subblock
 *          idempotent.
 * https://oeis.org/A224547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A224547: number of (n+1)×6 0/1 matrices with every 2×2 subblock idempotent.  
   Recurrence for n > 6:
        a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5)
   Initial values (offset 1):
        a(1) = 80, a(2) = 137, a(3) = 193, a(4) = 294,
        a(5) = 428, a(6) = 635
*/

#include <stdlib.h>

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset – behaviour there is irrelevant. */
    if (n < 1) return 0;

    /* Static cache for memoization. */
    static seq_t *cache = NULL;
    static size_t cached_last = 0;   /* last index computed and stored in cache, zero if uninitialised */
    static size_t cache_cap   = 0;   /* allocated capacity of cache array (size in seq_t elements) */

    /* Ensure the cache covers index n. */
    size_t target = (size_t)n;
    if (cache_cap <= target) {
        size_t new_cap = target + 1;              /* need indices 0..n inclusive */
        if (new_cap < 7) new_cap = 7;             /* guarantee space for first 6 terms */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – return 0 (unlikely in tests) */
            return 0;
        }
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Initialise first six terms if not already done. */
    if (cached_last == 0) {
        cache[1] = 80;
        cache[2] = 137;
        cache[3] = 193;
        cache[4] = 294;
        cache[5] = 428;
        cache[6] = 635;
        cached_last = 6;
    }

    /* Compute terms up to n if necessary. */
    for (size_t k = cached_last + 1; k <= target; ++k) {
        if (k <= 6) continue;   /* already initialised */
        /* Recurrence: a(k) = 4*a(k-1) - 5*a(k-2) + a(k-3) + 2*a(k-4) - a(k-5) */
        seq_t t4 = 4 * cache[k - 1];
        seq_t t5 = 5 * cache[k - 2];
        seq_t sumPos = t4 + cache[k - 3] + (2 * cache[k - 4]);  /* all additions are non‑negative */
        seq_t sumNeg = t5 + cache[k - 5];
        cache[k] = sumPos - sumNeg;    /* difference is guaranteed non‑negative */
    }

    cached_last = target;
    return cache[n];
}
