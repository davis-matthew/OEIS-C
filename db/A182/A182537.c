/*
 * A182537  a(0)=0, a(1)=1, a(n) = a(n-1) + (a(n-2) XOR n).
 * https://oeis.org/A182537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=0, a(1)=1, a(n) = a(n-1) + (a(n-2) XOR n)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoization cache: array of computed values. */
    static seq_t *cache = NULL;   /* allocated on first call */
    static size_t cap   = 0;      /* current capacity of cache   */
    static idx_t  last  = 1;      /* highest index known to be computed */

    /* Initialise cache for the base cases a(0)=0, a(1)=1. */
    if (!cache) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* fall back on allocation failure */
        cap   = 2;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* Grow the cache if required.  Note: using realloc is portable to
       C and C++.  The size is small (n ≤ 10 000 normally), so the overhead
       is negligible. */
    if ((size_t)(n + 1) > cap) {
        cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cap = (size_t)(n + 1);
    }

    /* Compute missing values up to n.  Prev1 = a(i-1), Prev2 = a(i-2). */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t add   = prev2 ^ (seq_t)i;   /* a(i-2) XOR i */
        cache[i]    = prev1 + add;        /* a(i-1) + add     */
    }

    /* Update the farthest computed index. */
    if (n > last) last = n;

    return cache[n];
}
