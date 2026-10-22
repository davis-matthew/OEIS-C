/*
 * A000711  Number of partitions of n, with three kinds of 1,2,3 and 4 and two
 *          kinds of 5,6,7,...
 * https://oeis.org/A000711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   EULER transform of 3,3,3,3,2,2,2,
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 50, u64 n <= 172, u128 n <= 633,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    seq_t *dp = (seq_t *)malloc((n+1) * sizeof(seq_t));
    if (dp == NULL) return 0;
    
    for (idx_t i = 0; i <= n; i++) dp[i] = 0;
    dp[0] = 1;
    
    for (idx_t k = 1; k <= n; k++) {
        int m = (k <= 4) ? 3 : 2;
        for (int iter = 0; iter < m; iter++) {
            for (idx_t i = k; i <= n; i++) {
                dp[i] += dp[i-k];
            }
        }
    }
    
    seq_t result = dp[n];
    free(dp);
    return result;
}
