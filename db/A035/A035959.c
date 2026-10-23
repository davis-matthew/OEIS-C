/*
 * A035959  Number of partitions of n in which no parts are multiples of 5.
 * https://oeis.org/A035959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 148, u64 n <= 494,
 *            u128 n <= 1762, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): number of partitions of n with no part divisible by 5.
   The values are obtained by dynamic programming:
     dp[0] = 1
     for every allowed part p (p % 5 != 0):
         for i = p .. n: dp[i] += dp[i - p]
   The array dp is extended only when a larger n is requested. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache: dp[i] = a(i) for 0 <= i <= max_n */
    static seq_t *dp = NULL;
    static size_t dp_size = 0;   /* allocated capacity */
    static idx_t  max_n   = 0;   /* largest n that has been computed */

    /* Initialise cache on first call */
    if (dp == NULL) {
        dp_size = 1;                          /* at least for i = 0 */
        dp = (seq_t *)realloc(NULL, dp_size * sizeof(seq_t));
        /* dp[0] = 1, other entries are zeroed implicitly here */
        dp[0] = 1;
        max_n = 0;
    }

    /* Extend the table if n is larger than what we have */
    if (n > max_n) {
        size_t newsize = (size_t)(n + 1);

        /* Allocate more space if necessary */
        if (newsize > dp_size) {
            seq_t *tmp = (seq_t *)realloc(dp, newsize * sizeof(seq_t));
            dp = tmp;
            dp_size = newsize;
        }

        /* Zero all entries except dp[0] (already 1) */
        for (idx_t i = 1; i <= n; ++i)
            dp[i] = 0;

        /* Compute partitions up to n */
        for (idx_t part = 1; part <= n; ++part) {
            if (part % 5 == 0)
                continue;                  /* skip multiples of 5 */
            for (idx_t i = part; i <= n; ++i) {
                dp[i] += dp[i - part];
            }
        }
        max_n = n;
    }

    return dp[n];
}
