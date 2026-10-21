/*
 * A087265  Lucas numbers L(8*n).
 * https://oeis.org/A087265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 2, a(1) = 47, a(n) = 47*a(n-1) - a(n-2) for n ≥ 2 (Lucas
 *            numbers L(8*n))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define A087265_CACHE_MAX 20000   /* should be ≥ 10000 */

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t cache[A087265_CACHE_MAX + 1];
    static bool initialized = false;
    static idx_t max_computed = 1;   /* highest index already filled */

    /* initialise cache on first call */
    if (!initialized) {
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)47;
        /* all other entries are zero‑inited by the compiler */
        initialized = true;
    }

    /* extend cache up to requested index (if necessary) */
    if (n > max_computed) {
        idx_t i;
        for (i = max_computed + 1; i <= n && i <= A087265_CACHE_MAX; ++i) {
            cache[i] = (seq_t)47 * cache[i - 1] - cache[i - 2];
        }
        if (i > max_computed)
            max_computed = i - 1;   /* highest computed index */
        /* If n > A087265_CACHE_MAX, the term is not cached.  Compute on the fly. */
    }

    /* If cached, just return */
    if (n <= A087265_CACHE_MAX)
        return cache[n];

    /* For indices beyond the preallocated cache (unlikely for tests),
       compute directly using the recurrence without storing the whole sequence. */
    seq_t prev2 = (seq_t)2;  /* a(0) */
    seq_t prev1 = (seq_t)47; /* a(1) */
    if (n == 0) return prev2;
    if (n == 1) return prev1;
    seq_t cur;
    unsigned long i;
    for (i = 2; i <= (unsigned long)n; ++i) {
        cur = (seq_t)47 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
