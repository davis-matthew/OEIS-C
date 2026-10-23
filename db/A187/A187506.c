/*
 * A187506  Let i be in {1,2,3,4} and let r >= 0 be an integer. Let p = {p_1,
 *          p_2, p_3, p_4} = {-1,0,1,2}, n=3*r+p_i, and define a(-1)=0. Then
 *          a(n)=a(3*r+p_i) gives the quantity of H_(9,4,0) tiles in a
 *          subdivided H_(9,i,r) tile after linear scaling by the factor Q^r,
 *          where Q=sqrt(x^3-2*x) with x=2*cos(Pi/9).
 * https://oeis.org/A187506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-3) + 3*a(n-6) - a(n-9) - a(n-12) Initial values:
 *            a(0)=0, a(1)=0, a(2)=1, a(3)=1, a(4)=1, a(5)=1, a(6)=2, a(7)=3,
 *            a(8)=4, a(9)=7, a(10)=9, a(11)=10 The sequence satisfies the
 *            recurrence for all n ≥ 12
 * Range:     u8 n <= 20, u16 n <= 35, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Memoised evaluation of the sequence. */
seq_t a(idx_t n)
{
    /* Persistent storage of computed values. */
    static seq_t *cache = NULL;      /* pointer to array of seq_t values   */
    static size_t cache_len = 0;     /* current allocated length          */
    static idx_t highest_computed = 0;  /* highest index that has been filled */

    /* (Re)allocate enough room for the requested index if necessary. */
    if ((size_t)n >= cache_len) {
        /* Grow the array to at least n+1 elements.  Use realloc to keep
           existing data.  The cast to size_t is safe because n ≤ 10000
           in the use‑cases.  */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) { /* allocation failed – leave return value undefined */
            return 0;
        }
        cache = tmp;
        cache_len = (size_t)(n + 1);

        /* Initialise or extend the cache with the first 12 terms
           if this is the first allocation. */
        if (highest_computed == 0) {
            /* If cache was previously NULL we fill the first 12 rows. */
            if (cache_len < 12) {
                /* Ensure there is space for the initial 12.  */
                cache = (seq_t *)realloc(cache, 12 * sizeof(seq_t));
                cache_len = 12;
            }
            cache[0] = 0;  /* a(0) */
            cache[1] = 0;  /* a(1) */
            cache[2] = 1;  /* a(2) */
            cache[3] = 1;  /* a(3) */
            cache[4] = 1;  /* a(4) */
            cache[5] = 1;  /* a(5) */
            cache[6] = 2;  /* a(6) */
            cache[7] = 3;  /* a(7) */
            cache[8] = 4;  /* a(8) */
            cache[9] = 7;  /* a(9) */
            cache[10] = 9; /* a(10) */
            cache[11] = 10;/* a(11) */
            highest_computed = 11;
        }
    }

    /* If the requested n is within the already computed range, return it. */
    if (n <= highest_computed) {
        return cache[(size_t)n];
    }

    /* Compute any missing terms up to n using the recurrence. */
    for (idx_t k = highest_computed + 1; k <= n; ++k) {
        /* The recurrence requires terms k-3, k-6, k-9, k-12.
           They are guaranteed to exist because k ≥ 12. */
        cache[(size_t)k] = 2 * cache[(size_t)(k - 3)]
                         + 3 * cache[(size_t)(k - 6)]
                         - cache[(size_t)(k - 9)]
                         - cache[(size_t)(k - 12)];
    }
    highest_computed = n;

    return cache[(size_t)n];
}
