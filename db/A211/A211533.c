/*
 * A211533  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w=3x-5y.
 * https://oeis.org/A211533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 627, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A211533: Number of ordered triples (w,x,y) with all terms in {1,…,n}
   and w = 3x – 5y.
   Recurrence (valid for n ≥ 9):
     a(n) = a(n-1) + a(n-3) – a(n-4) + a(n-5) – a(n-6) – a(n-8) + a(n-9)
   Initial values:
     a(0)=0, a(1)=0, a(2)=1, a(3)=1, a(4)=3, a(5)=4, a(6)=5, a(7)=8, a(8)=10
   Memoized implementation suitable for both C11 and C++17.                       */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for 0 … 8 */
    static const seq_t base[9] = {0, 0, 1, 1, 3, 4, 5, 8, 10};

    /* Cached sequence */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;   /* highest index filled + 1 */
    static size_t cache_cap   = 0;   /* allocated capacity */

    /* First call: allocate and initialise cache with the base values */
    if (!cache)
    {
        cache_cap = 16;                         /* modest start size */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                   /* allocation failure -> zero */
        for (size_t i = 0; i < 9; ++i)
            cache[i] = base[i];
        cache_size = 9;                         /* values 0 … 8 are ready */
    }

    /* If n is within the already computed portion, just return it */
    if (n < cache_size)
        return cache[(size_t)n];

    /* If n is still in the base range, return the constant value */
    if (n < 9)
        return base[(size_t)n];

    /* Ensure the cache is large enough for index n */
    if ((size_t)n >= cache_cap)
    {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap)
            new_cap <<= 1;                      /* grow until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                     /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute the sequence from the first missing index up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i)
    {
        /* The recurrence is only valid for i ≥ 9, but we start the loop
           at i = cache_size ≥ 9 after the initialisation above.          */
        seq_t val = 0;
        val += cache[i - 1];
        val += cache[i - 3];
        val -= cache[i - 4];
        val += cache[i - 5];
        val -= cache[i - 6];
        val -= cache[i - 8];
        val += cache[i - 9];
        cache[i] = val;
    }

    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
