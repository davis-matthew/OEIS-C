/*
 * A033121  Base-3 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,1.
 * https://oeis.org/A033121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-3) - 3*a(n-4) with a(1)=1, a(2)=3,
 *            a(3)=10, a(4)=31
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small cache for previously computed terms.  The cache is
       lazily initialized on the first call.  It always contains at
       least the first 4 terms of the sequence. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of terms stored (≥0)   */
    static idx_t cache_cap  = 0; /* allocated elements            */

    /* Handle trivial cases directly (n <= 4). */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)10;
    if (n == 4) return (seq_t)31;

    /* Ensure the cache is prepared for at least the first 4 terms. */
    if (cache_size == 0)
    {
        cache_cap  = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);        /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)31;
        cache_size = 4;
    }

    /* Make sure the cache is large enough to hold n terms. */
    if ((uint64_t)n > (uint64_t)cache_cap)
    {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Extend the cache up to term n. */
    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        seq_t val;
        idx_t idx = i - 1;   /* zero‑based index in the array */
        /* Compute using the recurrence.  For i > 4, idx >= 4. */
        val = (seq_t)3 * cache[idx - 1]     /* 3*a(i-1) */
            +           cache[idx - 3]    /*    a(i-3) */
            - (seq_t)3 * cache[idx - 4]; /* 3*a(i-4) */
        cache[idx] = val;
    }

    cache_size = n;
    return cache[n - 1];
}
