/*
 * A005490  Number of partitions of [n] where the first k elements are marked
 *          (0 <= k <= n-1) and at least k blocks contain their own index.
 * https://oeis.org/A005490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} b(n, i) where b(n, 1) = n and b(n+1, i+1) =
 *            (n-i) * b(n, i) + b(n+1, i)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    seq_t *prev = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    
    prev[1] = 1;
    seq_t result = 1;
    
    if (n == 1) {
        free(prev);
        free(curr);
        return result;
    }
    
    for (idx_t m = 1; m < n; m++) {
        curr[1] = (seq_t)(m + 1);
        
        for (idx_t i = 1; i <= m; i++) {
            curr[i + 1] = ((seq_t)(m - i)) * prev[i] + curr[i];
        }
        
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }
    
    result = 0;
    for (idx_t i = 1; i <= n; i++) {
        result += prev[i];
    }
    
    free(prev);
    free(curr);
    return result;
}
