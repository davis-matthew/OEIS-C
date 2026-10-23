/*
 * A181717  Fibonacci-Collatz sequence: a(1)=0, a(2)=1; for n>2, let
 *          fib=a(n-1)+a(n-2); if fib is odd then a(n)=3*fib+1 else
 *          a(n)=fib/2.
 * https://oeis.org/A181717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 30, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(1)=0, a(2)=1;
 * For n>2 let fib = a(n-1)+a(n-2);
 * If fib is odd then a(n)=3*fib+1 else a(n)=fib/2.
 * This is a second‑order recurrence that can be evaluated iteratively
 * with memoisation so that calls in arbitrary order are fast.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed values.
     * cache[0] is unused; indices start at 1. */
    static seq_t *cache = NULL;
    static idx_t   max_n = 0;   /* largest index we have computed */
    static size_t  cap   = 0;   /* allocated size of the cache array */

    /* Initialise cache on first call.  Allocate 3 slots: indices 0..2. */
    if (cache == NULL)
    {
        cap = 3;                      /* 0,1,2 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;         /* fallback: unlikely to happen */
        cache[0] = 0;                 /* unused */
        cache[1] = 0;                 /* a(1) */
        cache[2] = 1;                 /* a(2) */
        max_n = 2;
    }

    /* Expand cache if necessary */
    if ((size_t)n >= cap)
    {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < cap * 2) new_cap = cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* If request is within already computed range, return it */
    if (n <= max_n) return cache[n];

    /* Compute values up to n, extending the cache */
    for (idx_t i = max_n + 1; i <= n; ++i)
    {
        seq_t fib = cache[i - 1] + cache[i - 2];
        if ((fib & ((seq_t)1)) != 0)          /* fib is odd */
            cache[i] = fib * ((seq_t)3) + 1;
        else
            cache[i] = fib / 2;
    }
    max_n = n;
    return cache[n];
}
