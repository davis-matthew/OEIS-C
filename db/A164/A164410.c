/*
 * A164410  Number of binary strings of length n with no substrings equal to
 *          0000 0001 or 0101.
 * https://oeis.org/A164410
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) for n ≥ 7, with a(4)=13, a(5)=22,
 *            a(6)=38. This implements OEIS A164410: number of binary strings
 *            of length n having no substrings 0000, 0001, or 0101, offset 4
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 41, u64 n <= 83, u128 n <= 167,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoisation cache for computed values.  The cache holds indices
   from 0 up to the maximum n that has been requested so far. */
static seq_t *cache = NULL;     /* array of seq_t values */
static size_t cache_cap = 0;    /* allocated capacity of cache  */
static idx_t  max_computed = 0; /* largest index computed so far */

/* Compute a(n) for n ≥ 4, memoising results for future calls. */
seq_t a(idx_t n)
{
    /* Values for n < 4 are outside the offset; return 0. */
    if (n < 4) {
        return (seq_t)0;
    }

    /* Ensure the cache can contain index n.  Grow capacity exponentially
       to keep amortised O(1) time for successive expansions. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n + 1) {
            new_cap <<= 1;
        }
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Out of memory; return 0 as a safe failure value. */
            return (seq_t)0;
        }
        cache   = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise base values once.  These are required for the recurrence
       to fire from n = 7 onwards. */
    if (max_computed < 6) {
        cache[4] = (seq_t)13;
        cache[5] = (seq_t)22;
        cache[6] = (seq_t)38;
        max_computed = 6;
    }

    /* Compute any missing values up to requested n.  The loop is
       independent of the order of calls, so correctness holds for
       arbitrary call sequences. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Recurrence: a(i) = a(i-1) + 2 * a(i-3)  (valid for i ≥ 7) */
        cache[i] = cache[i - 1] + (seq_t)2 * cache[i - 3];
    }
    max_computed = n;

    return cache[n];
}
