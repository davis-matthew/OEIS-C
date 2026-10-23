/*
 * A170549  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 12, * a(n) = 11*a(n-1) - a(n-47) for n ≥ 2
 *            (with a(k)=0 for k<0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache     = NULL;   /* array of computed terms   */
    static size_t  size     = 0;      /* number of stored terms    */
    static size_t  capacity = 0;      /* allocated capacity        */

    /* initialise cache the first time */
    if (size == 0) {
        /* minimum needed to hold up to 47th term after the first expansion */
        capacity = (size_t)48;
        cache    = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out of memory; caller can treat as 0 */

        /* store known starting values */
        cache[0] = (seq_t)1;           /* a(0) */
        cache[1] = (seq_t)12;          /* a(1) */
        size = 2;
    }

    /* extend cache up to index n */
    while (size <= (size_t)n) {
        size_t i = size;               /* index of term to compute */

        /* ensure enough capacity */
        if (size == capacity) {
            capacity *= 2;             /* grow by factor of two */
            seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
            if (!tmp) return (seq_t)0; /* out of memory; caller can treat as 0 */
            cache = tmp;
        }

        /* recurrence: a(i) = 11*a(i-1) - a(i-47)  (a(k)=0 for k<0) */
        seq_t val = cache[i-1] * (seq_t)11;
        if (i >= 47)
            val -= cache[i-47];

        cache[i] = val;
        ++size;
    }

    return cache[n];
}
