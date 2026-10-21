/*
 * A083869  a(1)=1 then a(n) is the least k>=1 such that the nested radical
 *          sqrt(a(1)^2+sqrt(a(2)^2+sqrt(a(3)^2+(....+sqrt(a(n)^2)))...) is an
 *          integer.
 * https://oeis.org/A083869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A083869: a(1)=1; for n>1
 *   let t = n;
 *   for i = 1 .. n-1
 *       t = t*t - a(i)*a(i);
 *   a(n) = t
 *
 * The sequence is computed by memoizing the values of a(i) for all i ≤ n.
 * For each new n we recompute the term using the recurrence above.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Offset is 1; value for 0 is unused */

    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored */

    if (cache_size < n) {          /* Need to extend the cache up to n */
        /* Reallocate cache to hold n terms */
        cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        /* Compute all missing terms */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t t = (seq_t)i;    /* t1 = i */
            for (idx_t j = 1; j < i; ++j) {
                seq_t aj = cache[j-1];          /* a(j) */
                t = t * t - aj * aj;            /* t = t^2 - a(j)^2 */
            }
            cache[i-1] = t;   /* a(i) */
        }
        cache_size = n;
    }

    return cache[n-1];
}
