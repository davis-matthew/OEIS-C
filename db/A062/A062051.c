/*
 * A062051  Number of partitions of n into powers of 3.
 * https://oeis.org/A062051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into powers of 3 (unlimited
 *            multiplicity)
 * Range:     u8 n <= 83, u16 n <= 482, u32 n <= 5075, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp_cache = NULL;
static idx_t  dp_capacity = 0;

seq_t a(idx_t n)
{
    /* Grow cache if needed and recompute all values up to n
       using the standard coin‑change dynamic program */
    if (n >= dp_capacity) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);              /* memory exhausted */
        dp_cache = tmp;

        /* initialise to zero and set base case */
        for (idx_t i = 0; i < new_cap; ++i) dp_cache[i] = 0;
        dp_cache[0] = 1;

        /* iterate over all coin values 1,3,9,... up to n */
        idx_t coin = 1;
        while (coin <= n) {
            for (idx_t i = coin; i <= n; ++i) {
                dp_cache[i] += dp_cache[i - coin];
            }
            /* avoid overflow of coin *= 3 */
            if (coin > n / 3) break;
            coin *= 3;
        }
        dp_capacity = new_cap;
    }

    return dp_cache[n];
}
