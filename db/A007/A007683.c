/*
 * A007683  a(1) = 1; a(n) = -Sum_{k = 1..n-1} (n+k)!a(k)/(2k)!.
 * https://oeis.org/A007683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; a(n) = -Sum_{k = 1..n-1} (n+k)!*a(k)/(2k)!
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 12, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


static seq_t *memo = NULL;
static bool *computed = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    // Grow memo if needed
    if (n > memo_len) {
        idx_t new_len = (memo_len == 0) ? 16 : memo_len;
        while (new_len < n) new_len *= 2;
        
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        bool *new_computed = (bool *)malloc(new_len * sizeof(bool));
        
        if (memo != NULL) {
            memcpy(new_memo, memo, memo_len * sizeof(seq_t));
            memcpy(new_computed, computed, memo_len * sizeof(bool));
            free(memo);
            free(computed);
        }
        
        for (idx_t i = memo_len; i < new_len; i++) {
            new_memo[i] = 0;
            new_computed[i] = false;
        }
        
        memo = new_memo;
        computed = new_computed;
        memo_len = new_len;
    }
    
    // Return cached value if available
    if (computed[n-1]) {
        return memo[n-1];
    }
    
    // Compute a(n)
    seq_t result;
    if (n == 1) {
        result = 1;
    } else {
        seq_t sum = 0;
        for (idx_t k = 1; k < n; k++) {
            seq_t ak = a(k);
            
            // Compute (n+k)! / (2k)! = (2k+1) * (2k+2) * ... * (n+k)
            seq_t term = 1;
            for (idx_t i = 2*k + 1; i <= n + k; i++) {
                term *= (seq_t)i;
            }
            
            sum += term * ak;
        }
        result = -sum;
    }
    
    memo[n-1] = result;
    computed[n-1] = true;
    return result;
}
