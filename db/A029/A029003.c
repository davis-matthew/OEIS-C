/*
 * A029003  Expansion of 1/((1-x)*(1-x^2)*(1-x^3)*(1-x^9)).
 * https://oeis.org/A029003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 269, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: number of partitions of n using parts 1, 2, 3 and 9.
   The function uses a simple dynamic‑programming algorithm.
   For C++ builds a std::vector is used to ensure proper construction
   of arbitrary‑precision seq_t objects.  For C builds raw C allocation
   is used.  Complexity is O(n) per new maximum n, which is
   well below the 2‑second limit for n ≈ 10 000.  */

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

static const idx_t parts[4] = {1, 2, 3, 9};

seq_t a(idx_t n)
{
#ifdef __cplusplus
    static std::vector<seq_t> dp;
#else
    static seq_t *dp = NULL;
    static size_t cap = 0;
#endif

    size_t needed = (size_t)n + 1;

#ifdef __cplusplus
    if (dp.size() < needed) {
        dp.resize(needed);
        for (size_t i = 0; i < needed; ++i) dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;
        for (int k = 0; k < 4; ++k) {
            idx_t p = parts[k];
            for (idx_t i = p; i <= n; ++i)
                dp[(size_t)i] += dp[(size_t)(i - p)];
        }
    }
    return dp[(size_t)n];
#else
    if (cap < needed) {
        seq_t *tmp = (seq_t *)realloc(dp, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        dp = tmp;
        cap = needed;
        for (size_t i = 0; i < cap; ++i) dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;
        for (int k = 0; k < 4; ++k) {
            idx_t p = parts[k];
            for (idx_t i = p; i <= n; ++i)
                dp[(size_t)i] += dp[(size_t)(i - p)];
        }
    }
    return dp[(size_t)n];
#endif
}
