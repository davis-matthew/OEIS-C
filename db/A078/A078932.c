/*
 * A078932  Number of compositions (ordered partitions) of n into powers of 3.
 * https://oeis.org/A078932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n)=∑{k≥0 | 3^k≤n} a(n-3^k) for n>0
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 57, u64 n <= 112,
 *            u128 n <= 223, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of compositions of n into powers of 3 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp   = NULL;   /* table of a[0..] */
    static idx_t  size = 0;      /* number of elements allocated */

    /* initialise the table on first call */
    if (dp == NULL) {
        dp   = (seq_t *)malloc(sizeof(seq_t));   /* a[0] will be stored */
        dp[0] = (seq_t)1;                       /* a(0)=1 */
        size  = 1;
    }

    /* grow the table if necessary */
    if (n >= size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failed – abort */
            return (seq_t)0;
        }
        dp = tmp;

        /* initialise new entries to 0 and compute them */
        for (idx_t i = size; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            idx_t power = 1;          /* 3^0 */
            while (power <= i) {
                sum += dp[i - power];
                /* multiply power by 3 safely for idx_t */
                if (power > (unsigned long long)(-1) / 3) break; /* overflow guard */
                power *= 3;
            }
            dp[i] = sum;
        }
        size = new_size;
    }

    return dp[n];
}
