/*
 * A152895  Partial sums of A152891.
 * https://oeis.org/A152895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A152895 – Partial sums of A152891
 *
 *  Sequence:  a(n) = A001906(n+1) - A000217(n+1)
 *
 *  Recurrence (order 5) with initial values
 *      a(1)=0, a(2)=2, a(3)=11, a(4)=40, a(5)=123
 *
 *      a(n) = 6*a(n-1) - 13*a(n-2) + 13*a(n-3) - 6*a(n-4) + a(n-5)
 *
 *  This implementation memoises previously computed terms and
 *  returns the required value for any n≥1 in O(n) time (reusing
 *  already computed entries).  All arithmetic is performed in
 *  seq_t (the build‑time provided integer type), so the same code
 *  works for 8, 16, 32, 64 and 128‑bit unsigned types as well
 *  as arbitrary‑precision integers.
 */

#include <stdlib.h>

/* external linkage required */
seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i+1)  (0‑based) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms currently cached */

    /* enlarge cache if needed */
    if (cache_len < n)
    {
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        /* compute missing terms */
        for (idx_t i = cache_len; i < n; ++i)
        {
            /* initialise first five terms */
            if (i == 0)             cache[i] = 0;   /* a(1) */
            else if (i == 1)        cache[i] = 2;   /* a(2) */
            else if (i == 2)        cache[i] = 11;  /* a(3) */
            else if (i == 3)        cache[i] = 40;  /* a(4) */
            else if (i == 4)        cache[i] = 123; /* a(5) */
            else
            {
                /* recurrence for i >= 5 (term a(i+1)) */
                seq_t t1 = cache[i - 1];
                seq_t t2 = cache[i - 2];
                seq_t t3 = cache[i - 3];
                seq_t t4 = cache[i - 4];
                seq_t t5 = cache[i - 5];
                cache[i] = 6 * t1 - 13 * t2 + 13 * t3 - 6 * t4 + t5;
            }
        }
        cache_len = n;
    }

    return cache[n - 1];   /* cached value for a(n) */
}
