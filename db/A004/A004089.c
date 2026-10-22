/*
 * A004089  Reverse digits of number of partitions of n.
 * https://oeis.org/A004089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A004086(A000041(n)), reverse of digits of partition count
 * Range:     u8 n <= 13, u16 n <= 35, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (dp == NULL) return 0;
    
    for (idx_t i = 0; i <= n; i++) {
        dp[i] = 0;
    }
    dp[0] = 1;
    
    for (idx_t i = 1; i <= n; i++) {
        for (idx_t j = i; j <= n; j++) {
            dp[j] += dp[j - i];
        }
    }
    
    seq_t partitions = dp[n];
    free(dp);
    
    // Reverse the digits
    seq_t result = 0;
    seq_t temp = partitions;
    while (temp > 0) {
        result = result * 10 + temp % 10;
        temp /= 10;
    }
    
    return result;
}
