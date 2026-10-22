/*
 * A005220  Number of Dyck paths of knight moves.
 * https://oeis.org/A005220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, walk
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 28, u64 n <= 50, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute-force counting using dynamic programming: paths of knight moves from (0,0) to (n,0) that never go below y=0 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    idx_t max_y = 2 * n + 10;
    
    seq_t **dp = (seq_t **)malloc((n + 1) * sizeof(seq_t *));
    for (idx_t x = 0; x <= n; x++) {
        dp[x] = (seq_t *)calloc(max_y, sizeof(seq_t));
    }
    
    dp[0][0] = 1;
    
    for (idx_t x = 0; x < n; x++) {
        for (idx_t y = 0; y < max_y; y++) {
            if (dp[x][y] == 0) continue;
            
            // NNE: (x, y) -> (x+1, y+2)
            if (y + 2 < max_y) {
                dp[x + 1][y + 2] += dp[x][y];
            }
            
            // NEE: (x, y) -> (x+2, y+1)
            if (x + 2 <= n && y + 1 < max_y) {
                dp[x + 2][y + 1] += dp[x][y];
            }
            
            // SSE: (x, y) -> (x+1, y-2)
            if (y >= 2) {
                dp[x + 1][y - 2] += dp[x][y];
            }
            
            // SEE: (x, y) -> (x+2, y-1)
            if (x + 2 <= n && y >= 1) {
                dp[x + 2][y - 1] += dp[x][y];
            }
        }
    }
    
    seq_t result = dp[n][0];
    
    for (idx_t x = 0; x <= n; x++) {
        free(dp[x]);
    }
    free(dp);
    
    return result;
}
