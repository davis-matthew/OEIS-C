/*
 * A079971  Number of compositions (ordered partitions) of n into parts 1, 2,
 *          and 5.
 * https://oeis.org/A079971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 36 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A079971 – Number of compositions of n into parts 1, 2, and 5
 * Recurrence: a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=5
 *              a(n)=a(n-1)+a(n-2)+a(n-5)   for n>=5
 */

#include <stdlib.h>   /* calloc, realloc */
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache for dynamic programming */
    static seq_t *dp = NULL;     /* array of computed values */
    static idx_t  size = 0;     /* current allocated size */

    /* initialise cache with the five known terms */
    if (dp == NULL) {
        size = 5; /* indices 0..4 will be stored */
        dp = mem_new(seq_t, (size_t)size);
        if (dp == NULL) return (seq_t)0;  /* out of memory: unlikely in tests */

        dp[0] = (seq_t)1;
        dp[1] = (seq_t)1;
        dp[2] = (seq_t)2;
        dp[3] = (seq_t)3;
        dp[4] = (seq_t)5;
    }

    /* extend cache if we do not have a value for n yet */
    if (n >= size) {
        idx_t new_size = n + 1;
        seq_t *tmp = mem_resize(seq_t, dp, (size_t)new_size);
        if (tmp == NULL) return (seq_t)0;   /* out of memory: unlikely in tests */
        dp = tmp;

        /* compute the new terms using the recurrence */
        for (idx_t i = size; i <= n; ++i) {
            seq_t sum = dp[i-1] + dp[i-2];
            if (i >= 5) sum += dp[i-5];
            dp[i] = sum;
        }
        size = new_size;
    }

    return dp[n];
}
