/*
 * A142724  Irregular triangle read by rows: row n gives coefficients in
 *          expansion of Product_{k=1..n} (1 + x^(2*k + 1)) for n >= 0.
 * https://oeis.org/A142724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1337, u16 n <= 5151, u32 n <= 9993, u64 n <= 9993,
 *            u128 n <= 9993, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*
 * Formula: a(n) = the coefficient of x^e in
 *   Product_{k=1..r} (1 + x^(2*k + 1)),
 * where r is the row containing the index n in the flattened triangle
 * and e is the offset of n in that row.
 *
 * Initial values (a(0) = 1, a(1) = 1, a(2) = 0, ...) are automatically
 * produced by the algorithm; no explicit recurrence is used.
 */
seq_t a(idx_t n)
{
    /* Find the row r and the offset within that row */
    idx_t r = 0;
    idx_t cumulative = 0;
    while (1) {
        idx_t len = (r + 1) * (r + 1);      /* number of entries in row r */
        if (n < cumulative + len) {
            break;
        }
        cumulative += len;
        r++;
    }
    idx_t offset = n - cumulative;          /* position inside row r */

    /* Sum of all exponents in row r: r*(r+2) */
    idx_t maxSum = r * (r + 2);

    /* Dynamic programming array: dp[s] = number of subsets summing to s */
    size_t size = (size_t)(maxSum + 1);
    seq_t *dp = (seq_t *)malloc(size * sizeof(seq_t));
    if (!dp) {
        return (seq_t)0;                   /* allocation failure -> 0 */
    }
    for (size_t i = 0; i < size; ++i) {
        dp[i] = 0;
    }
    dp[0] = 1;                              /* empty subset */

    /* Add each odd weight 2*k+1 (k = 1..r) */
    for (idx_t k = 1; k <= r; ++k) {
        idx_t weight = 2 * k + 1;
        for (idx_t s = maxSum; s >= weight; --s) {
            dp[s] += dp[s - weight];
        }
    }

    seq_t result = dp[offset];
    free(dp);
    return result;
}
