/*
 * A208395  Number of 4 X n 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 61*a(n‑1) + 868*a(n‑2) + 2200*a(n‑3) - 7744*a(n‑4) -
 *            17728*a(n‑5) + 40704*a(n‑6) - 13824*a(n‑7) for n>9 Initial terms
 *            (OEIS): a(1)=14, a(2)=1094, a(3)=79092, a(4)=5800644,
 *            a(5)=424785708, a(6)=31112194860, a(7)=2278687944204,
 *            a(8)=166893600277548, a(9)=12223468411103052
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices before the offset */
    if (n < 1) return 0;

    /* Known values for 1 ≤ n ≤ 9 */
    static const seq_t init[10] = {
        0ULL,                     /* a(0) unused */
        14ULL,                    /* a(1)  */
        1094ULL,                  /* a(2)  */
        79092ULL,                 /* a(3)  */
        5800644ULL,               /* a(4)  */
        424785708ULL,             /* a(5)  */
        31112194860ULL,           /* a(6)  */
        2278687944204ULL,         /* a(7)  */
        166893600277548ULL,       /* a(8)  */
        12223468411103052ULL      /* a(9)  */
    };

    if (n <= 9) return init[n];

    /* Allocate a buffer large enough to hold entries up to n */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return 0;          /* allocation failed – return 0 */

    /* Seed the known initial values */
    for (idx_t i = 1; i <= 9; ++i) dp[i] = init[i];

    /* Use the recurrence for n ≥ 10 */
    for (idx_t i = 10; i <= n; ++i) {
        dp[i] =
            (seq_t)61   * dp[i - 1] +
            (seq_t)868  * dp[i - 2] +
            (seq_t)2200 * dp[i - 3] -
            (seq_t)7744 * dp[i - 4] -
            (seq_t)17728* dp[i - 5] +
            (seq_t)40704* dp[i - 6] -
            (seq_t)13824* dp[i - 7];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
