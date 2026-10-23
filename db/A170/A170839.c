/*
 * A170839  G.f.: Product_{k>=1} (1 + x^(2^k-1) + 3x^(2^k)).
 * https://oeis.org/A170839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 + x^(2^k-1) + 3 x^(2^k))
 * Range:     u8 n <= 59, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Allocate two arrays of length n+1 for dynamic programming. */
    size_t N = (size_t)n + 1;
    seq_t *dp   = alloc_seq_array(N);
    seq_t *next = alloc_seq_array(N);
    if (!dp || !next) {
        /* Out of memory – return 0 after freeing any allocated memory. */
        if (dp) free(dp);
        if (next) free(next);
        return 0;
    }

    /* Initialize dp[0] = 1, others 0. */
    dp[0] = 1;
    for (idx_t i = 1; i < N; ++i) dp[i] = 0;

    /* Iterate over each factor (1 + x^(2^k-1) + 3x^(2^k)). */
    for (idx_t k = 1; k < 64; ++k) {
        idx_t base   = ((idx_t)1) << k;   /* 2^k                          */
        idx_t offset1 = base - 1;         /* 2^k - 1                      */
        if (offset1 > n) break;           /* no more factors influence n   */
        idx_t offset2 = base;             /* 2^k                          */

        /* Zero the target array. */
        for (idx_t i = 0; i < N; ++i) next[i] = 0;

        /* Convolution step: add contributions from dp to next. */
        for (idx_t ii = 0; ii < N; ++ii) {
            seq_t val = dp[ii];
            if (!val) continue;

            /* constant term 1 */
            next[ii] += val;

            /* x^(2^k-1) term (coefficient 1) */
            idx_t j1 = ii + offset1;
            if (j1 < N) next[j1] += val;

            /* x^(2^k) term (coefficient 3) */
            idx_t j2 = ii + offset2;
            if (j2 < N) next[j2] += val + val + val;
        }

        /* Prepare for next factor: swap dp and next. */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    seq_t result = dp[n];

    /* Free allocated arrays. */
    free_seq_array(dp);
    free_seq_array(next);
    return result;
}
