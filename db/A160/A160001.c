/*
 * A160001  Number of partitions of n into distinct parts occurring in
 *          '3x+1'-trajectory starting with n.
 * https://oeis.org/A160001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts that occur
 *            in the Collatz (3x+1) trajectory of n. The parts are all
 *            distinct numbers ≤ n that appear in the trajectory starting from
 *            n, and each partition uses any subset of these parts whose sum
 *            equals n. The value is computed by dynamic progr...
 * Range:     u8 n <= 55, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Special case for n == 0 is not required; the sequence is defined
       only for n >= 1.  For safety, treat n == 0 as 0. */
    if (n == 0) return 0;

    /* Allocate arrays.  n fits in idx_t, so n+1 fits. */
    idx_t size = n + 1;
    unsigned char *visited = (unsigned char*)malloc((size) * sizeof(unsigned char));
    int *parts = (int*)malloc((size) * sizeof(int));
    seq_t *dp = (seq_t*)malloc((size) * sizeof(seq_t));

    if (!visited || !parts || !dp) {
        /* In a real implementation we would handle allocation failure,
           but here simply return 0. */
        free(visited); free(parts); free(dp);
        return 0;
    }

    /* Initialize arrays. */
    memset(visited, 0, size * sizeof(unsigned char));
    memset(dp, 0, size * sizeof(seq_t));
    dp[0] = 1;  /* one way to sum to zero */

    /* Step 1: collect all distinct numbers ≤ n that appear in the trajectory. */
    idx_t current = n;
    idx_t parts_len = 0;
    while (1) {
        if (current <= n && !visited[current]) {
            visited[current] = 1;
            parts[parts_len++] = (int)current;
        }
        if (current == 1) break;
        if (current & 1) {
            /* current is odd */
            current = current * 3 + 1;
        } else {
            /* current is even */
            current = current / 2;
        }
    }

    /* Step 2: dynamic programming for subset sum count. */
    for (idx_t i = 0; i < parts_len; ++i) {
        idx_t p = (idx_t)parts[i];  /* part value */
        for (idx_t sum = n; sum >= p; --sum) {
            dp[sum] += dp[sum - p];
        }
    }

    /* Result is dp[n]. */
    seq_t result = dp[n];

    /* Clean up */
    free(visited);
    free(parts);
    free(dp);

    return result;
}
