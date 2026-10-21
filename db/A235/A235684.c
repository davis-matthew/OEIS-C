/*
 * A235684  Number of compositions of n into powers of 3 and doubled powers of
 *          3.
 * https://oeis.org/A235684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 71, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n>=1,
   a(n) = Σ_{k≥0 , 3^k ≤ n} ( a(n-3^k) + a(n-2·3^k) ) */
#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are provided by the build. */
seq_t a(idx_t n)
{
    /* static DP array cached between calls */
    static seq_t *dp   = NULL;
    static idx_t  size = 0;

    /* initialise cache for first call */
    if (dp == NULL) {
        size = 1;
        dp   = (seq_t *)malloc(sizeof(seq_t));
        if (dp == NULL) abort();
        dp[0] = (seq_t)1;      /* a(0) = 1 */
    }

    /* extend cache if necessary */
    if (n >= size) {
        idx_t new_size = n + 1;
        seq_t *new_dp  = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (new_dp == NULL) abort();
        /* initialise newly allocated entries to 0 (uncomputed) */
        for (idx_t i = size; i < new_size; ++i) new_dp[i] = (seq_t)0;
        dp   = new_dp;
        size = new_size;
    }

    /* compute all values up to n that are still 0 */
    for (idx_t i = 1; i <= n; ++i) {
        if (dp[i] != (seq_t)0) continue;   /* already computed */

        seq_t sum = (seq_t)0;
        idx_t p   = 1;                     /* p = 3^k */

        while (p <= i) {
            /* part 3^k */
            sum += dp[i - p];

            /* part 2·3^k  only if fits */
            if (p <= i / 2) sum += dp[i - 2 * p];

            /* next power, guard against overflow */
            if (p > i / 3) break;
            p *= 3;
        }
        dp[i] = sum;
    }

    return dp[n];
}
