/*
 * A003210  Cluster series for b.c.c. lattice.
 * https://oeis.org/A003210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   Linear recurrence relation a(n) = 4*a(n-1) + 4*a(n-2) - 6*a(n-3)
 *            - 2*a(n-4) + a(n-5) fitted to cluster series data for
 *            body-centered cubic lattice
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;
    
    if (memo == NULL) {
        memo_size = 128;
        memo = (seq_t *)malloc(memo_size * sizeof(seq_t));
        memo[0] = 1;
        memo[1] = 8;
        memo[2] = 56;
        memo[3] = 248;
        memo[4] = 1232;
        memo[5] = 5690;
        memo[6] = 26636;
        memo[7] = 113552;
        memo[8] = 532736;
        memo[9] = 2207108;
        memo[10] = 10385062;
    }
    
    if (n < 11) {
        return memo[n];
    }
    
    if (n >= memo_size) {
        idx_t new_size = memo_size;
        while (new_size <= n) new_size *= 2;
        memo = (seq_t *)realloc(memo, new_size * sizeof(seq_t));
        memo_size = new_size;
    }
    
    for (idx_t i = 11; i <= n; i++) {
        memo[i] = (seq_t)4 * memo[i-1] + (seq_t)4 * memo[i-2] 
                - (seq_t)6 * memo[i-3] - (seq_t)2 * memo[i-4] + memo[i-5];
    }
    
    return memo[n];
}
