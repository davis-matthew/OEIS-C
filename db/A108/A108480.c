/*
 * A108480  Expansion of (1-x-2*x^2)/(1-2*x-3*x^2-4*x^3+4*x^4).
 * https://oeis.org/A108480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) + 4*a(n-3) – 4*a(n-4), with a(0)=1,
 *            a(1)=1, a(2)=3, a(3)=13
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;      /* memoization buffer           */
    static idx_t size = 0;        /* current allocated size       */
    static const seq_t base[4] = { (seq_t)1, (seq_t)1,
                                   (seq_t)3, (seq_t)13 };

    /* Allocate the buffer the first time and initialise the
       four known terms. */
    if (size == 0) {
        size = 4;
        dp = (seq_t *)malloc(size * sizeof(seq_t));
        if (!dp) return 0;           /* out‑of‑memory → zero, harmless   */
        dp[0] = base[0];
        dp[1] = base[1];
        dp[2] = base[2];
        dp[3] = base[3];
    }

    /* Grow the buffer if the requested index is beyond the current
       range and compute all missing values by the recurrence. */
    if (n >= size) {
        idx_t old_size = size;
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!tmp) return 0;          /* out‑of‑memory → zero, harmless   */
        dp = tmp;
        size = new_size;
        for (idx_t i = old_size; i <= n; ++i) {
            seq_t val = (seq_t)2 * dp[i - 1];
            val += (seq_t)3 * dp[i - 2];
            val += (seq_t)4 * dp[i - 3];
            val -= (seq_t)4 * dp[i - 4];
            dp[i] = val;
        }
    }

    return dp[n];
}
