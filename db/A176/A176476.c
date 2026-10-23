/*
 * A176476  Partial sums of A012814.
 * https://oeis.org/A176476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) + 5*a(n-3) - a(n-4) (a(0)=0, a(1)=1,
 *            a(2)=6, a(3)=27)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;            /* memoization array */
    static idx_t cached_size = 0;          /* allocated slots */
    static idx_t computed = 0;             /* number of slots already initialized */

    /* Grow cache if necessary */
    if (n + 1 > cached_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                 /* out of memory */
        cache = tmp;
        cached_size = (idx_t)new_size;
    }

    /* Compute missing values up to n */
    for (idx_t i = computed; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)0;
        } else if (i == 1) {
            cache[1] = (seq_t)1;
        } else if (i == 2) {
            cache[2] = (seq_t)6;
        } else if (i == 3) {
            cache[3] = (seq_t)27;
        } else {
            seq_t term = (seq_t)6 * cache[i - 1]
                         - (seq_t)9 * cache[i - 2]
                         + (seq_t)5 * cache[i - 3]
                         - cache[i - 4];
            cache[i] = term;
        }
    }
    computed = n + 1;                       /* update cache state */
    return cache[n];
}
