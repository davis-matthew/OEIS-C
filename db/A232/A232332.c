/*
 * A232332  Number of n X 5 0..2 arrays with every 0 next to a 1 and every 1
 *          next to a 2 horizontally or antidiagonally, with no adjacent
 *          elements equal.
 * https://oeis.org/A232332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for OEIS A232332
   a(1)=10, a(2)=82, a(3)=628, a(4)=4906, a(5)=38986
   for n>5: a(n)=11*a(n-1)-21*a(n-2)-20*a(n-3)-12*a(n-4) */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of a(1) .. a(k) */
static idx_t cache_len = 0;      /* number of elements stored */

/* Initial values (for n = 1 .. 5) */
static const seq_t init_vals[5] = {10, 82, 628, 4906, 38986};

seq_t a(idx_t n)
{
    if (n == 0u) return 0;       /* undefined for n=0 */

    if (n <= 5)
        return init_vals[n - 1];

    /* Grow the cache if necessary */
    if (cache_len < n) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t *)realloc((void *)cache,
                                       new_len * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failed */
        cache = tmp;

        /* Compute the new terms */
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            if (k <= 5) {
                cache[k - 1] = init_vals[k - 1];
            } else {
                /* recurrence: a(k) = 11*a(k-1)-21*a(k-2)-20*a(k-3)-12*a(k-4) */
                cache[k - 1] =
                    11 * cache[k - 2]
                  - 21 * cache[k - 3]
                  - 20 * cache[k - 4]
                  - 12 * cache[k - 5];
            }
        }
        cache_len = n;
    }
    return cache[n - 1];
}
