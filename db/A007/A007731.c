/*
 * A007731  a(n) = a(floor(n/2)) + a(floor(n/3)) + a(floor(n/6)), with a(0) =
 *          1.
 * https://oeis.org/A007731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(floor(n/2)) + a(floor(n/3)) + a(floor(n/6)), a(0) = 1
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    // Initialize memo array on first call
    if (memo == NULL) {
        memo_len = 10000;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        for (idx_t i = 0; i < memo_len; i++) {
            memo[i] = 0;
        }
    }
    
    // Expand memo array if needed
    if (n >= memo_len) {
        // Don't allocate huge arrays for very large n
        if (n > 1000000000) {
            return a(n / 2) + a(n / 3) + a(n / 6);
        }
        
        idx_t new_len = (n * 3) / 2 + 1;
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (new_memo == NULL) {
            // Allocation failed, compute without memoization
            return a(n / 2) + a(n / 3) + a(n / 6);
        }
        
        // Copy old values
        for (idx_t i = 0; i < memo_len; i++) {
            new_memo[i] = memo[i];
        }
        
        // Initialize new elements to 0
        for (idx_t i = memo_len; i < new_len; i++) {
            new_memo[i] = 0;
        }
        
        free(memo);
        memo = new_memo;
        memo_len = new_len;
    }
    
    // Return cached value if available
    if (memo[n] != 0) {
        return memo[n];
    }
    
    // Compute and cache result
    seq_t result = a(n / 2) + a(n / 3) + a(n / 6);
    memo[n] = result;
    return result;
}
