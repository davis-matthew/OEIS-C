/*
 * A078658  a(0) = 1; for n>0, a(n) = 1 + coefficient of x^n in expansion of
 *          1/Product_{ n >= 2, n not of the form 2^k-1 } (1-x^n).
 * https://oeis.org/A078658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + number of partitions of n into parts ≥2 that are not
 *            of the form 2^k-1
 * Range:     u8 n <= 29, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Returns true if k is of the form 2^t-1 (t ≥ 1).  For such k the part
   is forbidden.  Numbers of that form satisfy (k & (k+1)) == 0. */
static inline int is_forbidden(size_t k)
{
    return ((k & (k+1)) == 0);
}

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 1 */
        return 1;

    /* Convert to a type suitable for array indexing.   The size is
       reasonable (≤ 10000 for the intended tests). */
    size_t N = (size_t)n;

#ifdef __cplusplus
    /* In C++ use a vector so that element constructors are run. */
    std::vector<seq_t> dp(N + 1);
    dp[0] = 1;
    for (size_t i = 1; i <= N; ++i)
        dp[i] = 0;

    for (size_t k = 2; k <= N; ++k) {
        if (is_forbidden(k))
            continue;           /* skip forbidden part */
        for (size_t i = k; i <= N; ++i) {
            dp[i] += dp[i - k];
        }
    }
    return dp[N] + 1;            /* add the constant 1 */
#else
    /* In C use malloc.  The cast to seq_t* satisfies C++ as well. */
    seq_t *dp = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!dp)
        abort();                 /* allocation failure */

    dp[0] = 1;
    for (size_t i = 1; i <= N; ++i)
        dp[i] = 0;

    for (size_t k = 2; k <= N; ++k) {
        if (is_forbidden(k))
            continue;           /* skip forbidden part */
        for (size_t i = k; i <= N; ++i) {
            dp[i] += dp[i - k];
        }
    }
    seq_t result = dp[N] + 1;
    free(dp);
    return result;
#endif
}
