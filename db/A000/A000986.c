/*
 * A000986  Number of n X n symmetric matrices with (0,1) entries and all row
 *          sums 2.
 * https://oeis.org/A000986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*(n-1)*a(n-1) - (n-3)*(n-1)*a(n-2) - (n-3)*(n-2)*(n-1)/2 * a(n-4),
   with a(0) = 1, a(1) = 0, a(2) = 1, a(3) = 4 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;
    static idx_t computed_up_to = 3;
    
    // Expand dp array if needed
    if (n >= dp_size) {
        idx_t new_size = n + 1000;
        seq_t *new_dp = (seq_t *)malloc(new_size * sizeof(seq_t));
        
        if (dp != NULL) {
            for (idx_t i = 0; i < dp_size; i++) {
                new_dp[i] = dp[i];
            }
            free(dp);
        } else {
            // First allocation, initialize base cases
            new_dp[0] = 1;
            if (new_size > 1) new_dp[1] = 0;
            if (new_size > 2) new_dp[2] = 1;
            if (new_size > 3) new_dp[3] = 4;
        }
        
        dp = new_dp;
        dp_size = new_size;
    }
    
    // Compute values from computed_up_to + 1 to n
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        seq_t a1 = dp[i - 1];
        seq_t a2 = dp[i - 2];
        seq_t a4 = dp[i - 4];
        
        seq_t term1 = (seq_t)2 * ((seq_t)i - 1) * a1;
        seq_t term2 = ((seq_t)i - 3) * ((seq_t)i - 1) * a2;
        seq_t term3 = ((seq_t)i - 3) * ((seq_t)i - 2) * ((seq_t)i - 1) / (seq_t)2 * a4;
        
        dp[i] = term1 - term2 - term3;
    }
    
    if (n > computed_up_to) {
        computed_up_to = n;
    }
    
    return dp[n];
}
