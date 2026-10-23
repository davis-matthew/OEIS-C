/*
 * A034142  Number of partitions of n into distinct parts from [1, 12].
 * https://oeis.org/A034142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {1,2,…,12} whose elements sum to n
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_SUM 78     /* 1 + 2 + … + 12 */

seq_t a(idx_t n)
{
    /* static cache of all coefficients for sums 0..78 */
    static seq_t cache[MAX_SUM + 1];
    static bool computed = false;

    if (!computed) {
        /* initialise empty */
        for (idx_t i = 0; i <= MAX_SUM; ++i)
            cache[i] = 0;
        cache[0] = 1;               /* empty subset */

        /* dynamic programming over the 12 parts */
        for (idx_t part = 1; part <= 12; ++part) {
            for (idx_t sum = MAX_SUM; sum >= part; --sum) {
                cache[sum] += cache[sum - part];
            }
        }
        computed = true;
    }

    if (n <= MAX_SUM)
        return cache[n];
    else
        return 0;                   /* no subset can reach sums > 78 */
}
