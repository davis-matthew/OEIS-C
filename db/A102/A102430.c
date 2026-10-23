/*
 * A102430  Triangle read by rows where T(n,k) is the number of integer
 *          partitions of n > 1 into powers of k > 1.
 * https://oeis.org/A102430
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where r = ⌊(1 + √(1 + 8*(n-2))) / 2⌋ + 1 (row
 *            number, r ≥ 2) k = ((n-2) - (r-1)(r-2)/2) + 2 (column number, k
 *            ≥ 2) Recurrence for T(n,k) with n > 1, k > 1: T(1,k) = 1 T(n,k)
 *            = T(n-1,k) + (n % k == 0 ? T(n/k,k) : 0) All values are computed
 *            in type seq_t (built‑in or arbitrary‑precision)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#if defined(__cplusplus)
#  include <new>
#endif
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    /* The sequence starts at index 2. For n < 2 return 0 (undefined). */
    if (n < 2U) return 0;

    /* 1. Map linear index n to triangle coordinates (r,k). */
    idx_t offset = n - 2U;                     /* zero‑based position in sequence */
    idx_t sq = int_sqrt(1ULL + 8ULL * offset); /* integer floor of sqrt */
    idx_t r = (sq + 1ULL) / 2ULL + 1ULL;       /* row number, r ≥ 2 */
    idx_t prev_sum = (r - 1ULL) * (r - 2ULL) / 2ULL; /* total terms before row r */
    idx_t pos_in_row = offset - prev_sum;      /* 0‑based index within row r */
    idx_t k = pos_in_row + 2ULL;               /* column number, k ≥ 2 */

    /* 2. Compute T(r,k) using iterative DP. */
    /* Allocate an array of seq_t of size r+1 (indices 0..r). */
#if defined(__cplusplus)
    seq_t *dp = new seq_t[r + 1];
#else
    seq_t *dp = (seq_t *)malloc(sizeof(seq_t) * (size_t)(r + 1));
#endif
    if (!dp) return 0; /* allocation failure */

    /* Base case: T(1,k) = 1. */
    dp[1] = 1;

    /* Build up to row r. */
    for (idx_t m = 2ULL; m <= r; ++m) {
        seq_t val = dp[m - 1];               /* T(m-1,k) */
        if (m % k == 0ULL) {
            val += dp[m / k];                /* add T(m/k,k) */
        }
        dp[m] = val;
    }

    seq_t result = dp[r];

    /* Free the allocated array. */
#if defined(__cplusplus)
    delete[] dp;
#else
    free(dp);
#endif

    return result;
}
