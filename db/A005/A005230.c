/*
 * A005230  Stern's sequence: a(1) = 1, a(n+1) is the sum of the m preceding
 *          terms, where m*(m-1)/2 < n <= m*(m+1)/2 or equivalently m =
 *          ceiling((sqrt(8*n+1)-1)/2) = A002024(n).
 * https://oeis.org/A005230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  core, easy, nonn, nice
 * Formula:   a(1) = 1, a(n+1) = sum of m preceding terms, where m*(m-1)/2 < n
 *            <= m*(m+1)/2
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *memo = NULL;
static idx_t memo_computed = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    if (n == 1) {
        if (memo_computed == 0) {
            if (memo == NULL) {
                memo = (seq_t *)malloc(sizeof(seq_t));
            }
            memo[0] = 1;
            memo_computed = 1;
        }
        return memo[0];
    }
    
    if (n > memo_computed) {
        // Reallocate if needed
        if (memo == NULL) {
            memo = (seq_t *)malloc(n * sizeof(seq_t));
        } else {
            seq_t *new_memo = (seq_t *)malloc(n * sizeof(seq_t));
            memcpy(new_memo, memo, memo_computed * sizeof(seq_t));
            free(memo);
            memo = new_memo;
        }
        
        // Initialize first element if not done
        if (memo_computed == 0) {
            memo[0] = 1;
            memo_computed = 1;
        }
        
        // Compute values from memo_computed to n-1
        for (idx_t i = memo_computed; i < n; i++) {
            // Find m such that m*(m-1)/2 < i <= m*(m+1)/2
            idx_t m = 1;
            while (m * (m + 1) / 2 < i) {
                m++;
            }
            
            // Sum last m terms: a(i) + a(i-1) + ... + a(i-m+1)
            seq_t sum = 0;
            for (idx_t j = 0; j < m; j++) {
                sum += memo[i - 1 - j];
            }
            memo[i] = sum;
        }
        memo_computed = n;
    }
    
    return memo[n - 1];
}
