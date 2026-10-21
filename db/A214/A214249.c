/*
 * A214249  Number A(n,k) of compositions of n where differences between
 *          neighboring parts are in {-k,...,k} \ {0}; square array A(n,k),
 *          n>=0, k>=0, read by antidiagonals.
 * https://oeis.org/A214249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read from the square array A(i,j) where i is the sum of
 *            the composition and j is the allowed maximum absolute difference
 *            between consecutive parts. The map from index n to (i,j) is: s =
 *            floor((sqrt(8*n+1)-1)/2) // antidiagonal number i = n -
 *            s*(s+1)/2 j = s - i Then A(i,j) counts the comp...
 * Range:     u8 n <= 147, u16 n <= 399, u32 n <= 1217, u64 n <= 4086,
 *            u128 n <= 9525, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: The function works for any seq_t / idx_t definitions
   provided by the build system. */

#include <stdlib.h>
#include <math.h>

/* Count compositions of n with max adjacent difference k.
   Recurrence: dp[sum][last] = number of compositions of 'sum'
   ending with part 'last'.  */
static seq_t count_compositions(idx_t n, idx_t k)
{
    if (n == 0) return 1;          /* empty composition */

    size_t N = (size_t)n + 1;      /* dimension size for dp */
    size_t total = N * N;
    seq_t *dp = (seq_t *)calloc(total, sizeof(seq_t));
    if (!dp) return 0;             /* allocation failed */

    /* single–part compositions */
    for (idx_t t = 1; t <= n; ++t)
        dp[t * N + t] = 1;

    /* Build larger compositions */
    for (idx_t s = 1; s < n; ++s) {
        for (idx_t last = 1; last <= s; ++last) {
            seq_t cur = dp[s * N + last];
            if (cur == 0) continue;

            /* determine allowed next parts */
            idx_t next_min = (last > k) ? last - k : 1;
            idx_t next_max = last + k;
            /* cannot exceed remaining sum */
            if (next_max > n - s) next_max = n - s;
            if (next_min > next_max) continue;

            for (idx_t next = next_min; next <= next_max; ++next) {
                if (next == last) continue;      /* difference must be non–zero */
                dp[(s + next) * N + next] += cur;
            }
        }
    }

    /* sum over ending parts */
    seq_t total_count = 0;
    for (idx_t last = 1; last <= n; ++last)
        total_count += dp[n * N + last];

    free(dp);
    return total_count;
}

seq_t a(idx_t n)
{
    /* find antidiagonal number s */
    double temp = sqrt(8.0 * (double)n + 1.0);
    idx_t s = (idx_t)((temp - 1.0) / 2.0);     /* floor implicitly */

    /* compute (i,j) */
    idx_t start = s * (s + 1) / 2;
    idx_t i = n - start;      /* row index – sum of parts */
    idx_t j = s - i;          /* column index – max difference */

    return count_compositions(i, j);
}
