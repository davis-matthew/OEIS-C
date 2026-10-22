/*
 * A003263  Number of representations of n as a sum of distinct Lucas numbers
 *          1, 3, 4, 7, 11, ... (A000204).
 * https://oeis.org/A003263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 307, u16 n <= 9349, u32 n <= 9349, u64 n <= 9349,
 *            u128 n <= 9349, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Method: DP to count representations as sums of distinct Lucas numbers (1,3,4,7,11,...). */

seq_t a(idx_t n) {
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    
    for (idx_t i = 0; i <= n; i++) {
        dp[i] = (seq_t)0;
    }
    
    seq_t lucas[200];
    int lucas_count = 0;
    
    lucas[0] = (seq_t)1;
    lucas[1] = (seq_t)3;
    lucas_count = 2;
    
    while (lucas_count < 200) {
        seq_t next = lucas[lucas_count - 1] + lucas[lucas_count - 2];
        if (next > (seq_t)n) break;
        lucas[lucas_count] = next;
        lucas_count++;
    }
    
    dp[0] = (seq_t)1;
    
    for (int i = 0; i < lucas_count; i++) {
        seq_t L = lucas[i];
        for (idx_t j = n; j >= (idx_t)L; j--) {
            dp[j] = dp[j] + dp[j - (idx_t)L];
        }
    }
    
    seq_t result = dp[n];
    free(dp);
    return result;
}
