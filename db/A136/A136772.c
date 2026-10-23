/*
 * A136772  n! never ends in this many 0's in base 12.
 * https://oeis.org/A136772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = n-th positive integer that never occurs as the number *
 *            of trailing zeros of m! in base 12, i.e. as min(v₂(m!)/2,
 *            v₃(m!))
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined for n = 0, but return 0 */
        return (seq_t)0;

    /* Static cache of missing values. */
    static idx_t *missing      = NULL;
    static size_t miss_cnt     = 0;   /* how many missing values stored            */
    static size_t miss_cap     = 0;   /* allocated capacity of missing array        */

    /* State of the longest computed factorial. */
    static idx_t current_n      = 0;   /* last n processed                        */
    static idx_t v2             = 0;   /* exponent of 2 in current_n!             */
    static idx_t v3             = 0;   /* exponent of 3 in current_n!             */
    static idx_t prevZ          = (idx_t)-1;  /* last trailing‑zero count                */

    /* Grow the cache until it contains at least n missing values. */
    while (miss_cnt < n)
    {
        /* Advance to the next m. */
        ++current_n;

        /* Add the exponents contributed by current_n to factorial. */
        idx_t t = current_n;
        while ((t & 1) == 0) { ++v2; t >>= 1; }          /* factor 2 */
        t = current_n;
        while (t % 3 == 0) { ++v3; t /= 3; }             /* factor 3 */

        /* Compute Z(current_n) = trailing zero count in base 12. */
        idx_t half = v2 >> 1;                            /* v2 / 2   */
        idx_t z    = (half < v3) ? half : v3;            /* min( , ) */

        /* Record missing integers between prevZ+1 and z-1. */
        if (prevZ != (idx_t)-1) {                         /* not first iteration */
            for (idx_t m = prevZ + 1; m < z; ++m)
            {
                if (miss_cnt >= miss_cap) {
                    miss_cap = miss_cap ? miss_cap << 1 : 256;
                    missing = (idx_t *)realloc(missing, miss_cap * sizeof(idx_t));
                    if (missing == NULL) abort();       /* allocation failed */
                }
                missing[miss_cnt++] = m;
            }
        } else {                                         /* first iteration */
            for (idx_t m = 1; m < z; ++m)
            {
                if (miss_cnt >= miss_cap) {
                    miss_cap = miss_cap ? miss_cap << 1 : 256;
                    missing = (idx_t *)realloc(missing, miss_cap * sizeof(idx_t));
                    if (missing == NULL) abort();
                }
                missing[miss_cnt++] = m;
            }
        }

        prevZ = z;
    }

    /* Return the requested term. */
    return (seq_t)missing[n - 1];
}
