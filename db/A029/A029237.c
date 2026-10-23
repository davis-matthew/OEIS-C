/*
 * A029237  Expansion of 1/((1-x^2)*(1-x^8)*(1-x^9)*(1-x^11)).
 * https://oeis.org/A029237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 8, 9, and 11
 * Range:     u8 n <= 119, u16 n <= 839, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by dynamic programming with O(n) time and O(n) memory. */

#include <stdlib.h>   /* For malloc / free (C) and std::size_t (C++) */

seq_t a(idx_t n) {
    /* Parts that may be used in the partitions */
    const idx_t parts[4] = {2, 8, 9, 11};

    /* Allocate space for dp[0..n] */
#if defined(__cplusplus)
    /* C++: use new[] to construct any default‑constructible seq_t type */
    seq_t *dp = new seq_t[n + 1];
#else
    /* C: use malloc for plain data types */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (dp == NULL) { /* Handle allocation failure gracefully */
        return 0;
    }
#endif

    /* Initialise dp array to zero */
    for (idx_t i = 0; i <= n; ++i) {
        dp[i] = 0;
    }
    dp[0] = 1;  /* Empty partition */

    /* Standard unbounded knapsack DP */
    for (int p = 0; p < 4; ++p) {
        idx_t part = parts[p];
        if (part > n) {
            continue;
        }
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];

    /* Free the allocated memory */
#if defined(__cplusplus)
    delete[] dp;
#else
    free(dp);
#endif

    return result;
}
