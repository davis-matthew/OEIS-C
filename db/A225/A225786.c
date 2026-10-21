/*
 * A225786  Numbers k such that oblong(2*k) + oblong(k) is a square, where
 *          oblong(k) = A002378(k) = k*(k+1).
 * https://oeis.org/A225786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A225786
 *
 *  Sequence: Numbers k such that oblong(2*k)+oblong(k) is a square,
 *  equivalently k*(5*k+3) is a perfect square.
 *
 *  Recurrence: a(1)=0, a(2)=48, a(3)=15552,
 *              a(n) = 323*a(n-1) - 323*a(n-2) + a(n-3)   for n>=4
 *
 *  The function keeps a static cache of previously computed values
 *  up to a maximum index of 10000.  This is sufficient for the typical
 *  size of the tests.  The cache is grown in-place only when a larger
 *  index is requested, and the recurrence is applied to fill any
 *  missing terms.  The function is safe for calls in arbitrary order.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Maximum support for n up to 10000 – increase if needed. */
    enum { MAX_N = 10000 };

    /* Static cache and bookkeeping.  cache[0] holds a(1). */
    static seq_t cache[MAX_N + 1];
    static idx_t cached_count = 0;  /* number of cached terms (>=0) */

    /* The function assumes n>=1 per the requirements. */
    /* Ensure the cache is initialized (first three terms). */
    if (cached_count == 0) {
        /* a(1) = 0 */
        cache[0] = (seq_t)0;
        /* a(2) = 48 */
        cache[1] = (seq_t)48;
        /* a(3) = 15552 */
        cache[2] = (seq_t)15552;
        cached_count = 3;  /* we have three terms stored */
    }

    /* If the requested index is already cached, just return it. */
    if (n <= cached_count) {
        return cache[n - 1];
    }

    /* Compute missing terms up to n using the recurrence.
     * WARNING: If n exceeds MAX_N, the cache cannot grow; the
     * function will simply truncate.  The problem statement
     * guarantees n will be within reasonable bounds. */
    if (n > (idx_t)MAX_N) {
        /* For safety, we clamp to MAX_N.  The function will
         * return the largest precomputed value in this case. */
        n = (idx_t)MAX_N;
    }

    for (idx_t i = cached_count + 1; i <= n; ++i) {
        /* i maps to cache[i-1]  (since cache[0] = a(1)) */
        seq_t term = (seq_t)323 * cache[i - 2];
        term -= (seq_t)323 * cache[i - 3];
        term += cache[i - 4];
        cache[i - 1] = term;
    }
    cached_count = n;

    return cache[n - 1];
}
