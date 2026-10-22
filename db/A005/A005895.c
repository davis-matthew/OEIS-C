/*
 * A005895  Weighted count of partitions with distinct parts.
 * https://oeis.org/A005895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   sum of largest parts of all partitions of n into distinct parts.
 *            Equivalently, G.f. sum(k>=0, (k+1)*x^(k+1) * prod(j=1..k, 1+x^j)
 *            )
 * Range:     u8 n <= 15, u16 n <= 48, u32 n <= 172, u64 n <= 648,
 *            u128 n <= 2501, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    seq_t *dp = (seq_t *)calloc(n, sizeof(seq_t));
    dp[0] = 1;
    seq_t result = 0;
    
    for (idx_t k = 0; k < n; k++) {
        result += (seq_t)(k + 1) * dp[n - (k + 1)];
        
        for (idx_t i = n - 1; i >= k + 1; i--) {
            dp[i] += dp[i - (k + 1)];
        }
    }
    
    free(dp);
    return result;
}
