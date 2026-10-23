/*
 * A022363  Fibonacci sequence beginning 0, 29.
 * https://oeis.org/A022363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 29, a(n) = a(n-1) + a(n-2) (n ≥ 2)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This satisfies the OEIS definition of A022363. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: memo[0..last] contain computed values */
    static seq_t *memo  = NULL;
    static size_t   cap   = 0;    /* allocated capacity */
    static idx_t    last  = 0;    /* last index that has been computed */

    /* initialisation on first call */
    if (memo == NULL)
    {
        /* allocate room for the first two terms */
        cap   = 2;
        memo  = (seq_t *)malloc(cap * sizeof(seq_t));
        if (memo == NULL) return (seq_t)0;   /* allocation failure fallback */

        memo[0] = (seq_t)0;
        memo[1] = (seq_t)29;
        last    = 1;   /* indices 0 and 1 are computed */
    }

    /* ensure the array is large enough to hold memo[n] */
    size_t needed = (size_t)n + 1;  /* +1 for zero‑based indexing */
    if (needed > cap)
    {
        size_t new_cap = cap ? cap << 1 : 2;
        while (new_cap < needed)
            new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure fallback */

        memo = tmp;
        cap  = new_cap;
    }

    /* compute any missing terms up to n */
    if (n > last)
    {
        for (idx_t i = last + 1; i <= n; ++i)
            memo[i] = memo[i - 1] + memo[i - 2];

        last = n;
    }

    return memo[n];
}
