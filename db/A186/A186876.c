/*
 * A186876  Number of (n+2) X 6 0..3 arrays with every 3 X 3 subblock
 *          commuting with each horizontal and vertical neighbor 3 X 3
 *          subblock.
 * https://oeis.org/A186876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence a(n) = 2*a(n-1) - a(n-2) +
 *            3*a(n-3) - 6*a(n-4) + 3*a(n-5) + 3*a(n-6) - 3*a(n-7) with
 *            initial values a(1)..a(12) given in the OEIS entry
 * Range:     u8 none, u16 n <= 3, u32 n <= 28, u64 n <= 71, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t init_vals[13] = {
    0ULL,          // 0 (unused, offset 1)
    46583ULL,      // a(1)
    6829ULL,       // a(2)
    20152ULL,      // a(3)
    79096ULL,      // a(4)
    100600ULL,     // a(5)
    108640ULL,     // a(6)
    121144ULL,     // a(7)
    148048ULL,     // a(8)
    208576ULL,     // a(9)
    364504ULL,     // a(10)
    628144ULL,     // a(11)
    1130851ULL     // a(12)
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1 */

    /* For n <= 12 we can return directly. */
    if (n <= 12) return init_vals[(size_t)n];

    /* Static memoisation */
    static seq_t *cache      = NULL;   /* array holding computed values */
    static size_t cache_alloc = 0;     /* allocated length (>= n+1) */
    static size_t cache_max   = 0;     /* largest index already computed */

    size_t idx = (size_t)n;

    /* Grow the cache if necessary. */
    if (idx + 1 > cache_alloc) {
        size_t old_alloc = cache_alloc;
        cache_alloc = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_alloc * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* Zero the new part of the array (old values are preserved). */
        memset(&cache[old_alloc], 0, (cache_alloc - old_alloc) * sizeof(seq_t));

        /* Initialise the first 12 entries (indices 1..12). */
        for (size_t i = 1; i <= 12 && i < cache_alloc; ++i)
            cache[i] = init_vals[i];
    }

    /* Compute missing values up to idx. */
    for (size_t i = cache_max + 1; i <= idx; ++i) {
        if (i <= 12) continue; /* already initialised */

        /* Compute using only addition/subtraction to avoid overflow
           on 64‑bit builds that do not exceed the limits of a(n). */
        seq_t val = cache[i-1];
        val += cache[i-1];                // +2*a(i-1)
        val -= cache[i-2];                // -a(i-2)
        val += cache[i-3]; val += cache[i-3]; val += cache[i-3];   // +3*a(i-3)
        val -= cache[i-4]; val -= cache[i-4]; val -= cache[i-4];
        val -= cache[i-4]; val -= cache[i-4]; val -= cache[i-4]; // -6*a(i-4)
        val += cache[i-5]; val += cache[i-5]; val += cache[i-5];   // +3*a(i-5)
        val += cache[i-6]; val += cache[i-6]; val += cache[i-6];   // +3*a(i-6)
        val -= cache[i-7]; val -= cache[i-7]; val -= cache[i-7];   // -3*a(i-7)

        cache[i] = val;
    }
    cache_max = idx > cache_max ? idx : cache_max;

    return cache[idx];
}
