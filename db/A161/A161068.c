/*
 * A161068  Number of partitions of n into powers of two minus one where every
 *          part appears at least 6 times.
 * https://oeis.org/A161068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into powers of two minus one where
 *            each part appears at least 6 times
 * Range:     u8 n <= 145, u16 n <= 540, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   free_seq_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/free_seq_arr.h"

/* helper for dynamic allocation that works in C and C++ */
static seq_t* alloc_seq_arr(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t*)malloc(n * sizeof(seq_t));
#endif
}

/* seq_t a(idx_t n)   computes A161068 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    size_t N = (size_t)n;          /* safe because n ≤ 10000 in tests */
    size_t size = N + 1;

    seq_t *dp = alloc_seq_arr(size);
    dp[0] = 1;
    for (size_t i = 1; i <= N; ++i) dp[i] = 0;

    /* iterate over powers of two minus one: 1,3,7,15,... */
    for (uint64_t k = 1; ; ++k)
    {
        uint64_t p = ((uint64_t)1 << k) - 1;   /* 2^k - 1 */
        if (p > (uint64_t)N) break;

        seq_t *new_dp = alloc_seq_arr(size);

        for (uint64_t r = 0; r < p && r <= (uint64_t)N; ++r)
        {
            size_t count = (N - r) / p + 1;
            seq_t *prefix = alloc_seq_arr(count);
            seq_t acc = 0;
            for (size_t ii = 0; ii < count; ++ii)
            {
                size_t idx = r + ii * p;
                acc += dp[idx];
                prefix[ii] = acc;
            }
            for (size_t ii = 0; ii < count; ++ii)
            {
                size_t idx = r + ii * p;
                seq_t val = dp[idx];
                if (ii >= 6) val += prefix[ii - 6];
                new_dp[idx] = val;
            }
            free_seq_arr(prefix);
        }

        free_seq_arr(dp);
        dp = new_dp;
    }

    seq_t result = dp[N];
    free_seq_arr(dp);
    return result;
}
