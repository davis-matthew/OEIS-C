/*
 * A032032  Number of ways to partition n labeled elements into sets of sizes
 *          of at least 2 and order the sets.
 * https://oeis.org/A032032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2..n} binom(n,k) * a(n-k) with base values a(0)=1,
 *            a(1)=0
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoized values */
    static size_t cap    = 0;     /* number of cached entries */

    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        /* realloc accepts void*, so cast to the proper type */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

        /* initialise new slots to 0 */
        for (size_t i = cap; i < new_cap; ++i)
            cache[i] = 0;

        /* compute all values up to n */
        for (size_t i = cap; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[0] = 1;
            } else if (i == 1) {
                cache[1] = 0;
            } else {
                seq_t sum = 0;
                seq_t C   = 1;            /* binom(i,0) */
                for (size_t k = 1; k <= i; ++k) {
                    /* C = binom(i,k) from binom(i,k-1) */
                    C = (C * (seq_t)(i - k + 1)) / (seq_t)k;
                    if (k >= 2)            /* use only k>=2 */
                        sum += C * cache[i - k];
                }
                cache[i] = sum;
            }
        }
        cap = new_cap;
    }
    return cache[(size_t)n];
}
