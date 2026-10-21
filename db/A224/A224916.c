/*
 * A224916  Expansion of chi(x)^2 / chi(-x^2)^6 in powers of x where chi() is
 *          a Ramanujan theta function.
 * https://oeis.org/A224916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 20, u32 n <= 60, u64 n <= 200, u128 n <= 710,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A224916
   Formula: a(n) = coefficient of ∏_{k>0} (1 + x^k)^2 * (1 + x^(2k))^4 in powers of x
   The sequence starts with a(0)=1, a(1)=2, a(2)=7, ...
   This implementation builds the coefficients by iterative convolution.
   Complexity: O(n²) per call, fast enough for n ≤ 10000. */

#include <stdlib.h>

/* Coefficients of the polynomial (1+x^k)^2 * (1+x^(2k))^4 for each k
   | t*k | coefficient
   |  0  |   1         (coeffs[0] not used in convolution)
   |  k  |   2         coeffs[1]
   | 2k  |   5         coeffs[2]
   | 3k  |   8         coeffs[3]
   | 4k  |  10         coeffs[4]
   | 5k  |  12         coeffs[5]
   | 6k  |  10         coeffs[6]
   | 7k  |   8         coeffs[7]
   | 8k  |   5         coeffs[8]
   | 9k  |   2         coeffs[9]
   |10k  |   1         coeffs[10] */
static const unsigned int coeffs[11] = {1, 2, 5, 8, 10, 12, 10, 8, 5, 2, 1};

/* Memoised coefficient array shared among all calls */
static seq_t *dp = NULL;
static size_t dp_cap = 0;

/* Return a(n) for the sequence A224916 */
seq_t a(idx_t n)
{
    /* Ensure dp has room for indices 0..n */
    if (dp_cap < (size_t)(n + 1)) {
        seq_t *tmp = (seq_t *)realloc(dp, (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* Allocation failed */
        dp = tmp;
        dp_cap = (size_t)(n + 1);
    }

    /* Initialise dp[0] = 1, dp[1..n] = 0 */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    /* Perform convolution for each k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k) {
        /* Descending loop over j to avoid interference within the same k */
        for (idx_t j = n; ; --j) {
            seq_t val = dp[j];
            if (val) {
                /* Add contributions with t = 1 .. 10 */
                for (int t = 1; t <= 10; ++t) {
                    idx_t new_j = j + k * (idx_t)t;
                    if (new_j > n) break;
                    dp[new_j] += val * (seq_t)coeffs[t];
                }
            }
            if (j == 0) break;   /* Finished loop */
        }
    }

    return dp[n];
}
