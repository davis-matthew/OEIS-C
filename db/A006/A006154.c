/*
 * A006154  Number of labeled ordered partitions of an n-set into odd parts.
 * https://oeis.org/A006154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=1..ceiling(n/2)} C(n,2*k-1)*a(n-2*k+1), a(0) =
 *            a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   a000707_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a000707_binom.h"

static seq_t *memo = NULL;
static idx_t memo_size = 0;

seq_t a(idx_t n) {
    if (n <= 1) return 1;
    
    // Expand memo array if needed
    if (n >= memo_size) {
        idx_t new_size = n + 1;
        seq_t *new_memo = (seq_t *)malloc(new_size * sizeof(seq_t));
        
        if (memo) {
            for (idx_t i = 0; i < memo_size; i++) {
                new_memo[i] = memo[i];
            }
            free(memo);
        }
        
        for (idx_t i = memo_size; i < new_size; i++) {
            new_memo[i] = 0;
        }
        
        memo = new_memo;
        memo_size = new_size;
    }
    
    // Return cached result if available
    if (memo[n] != 0) return memo[n];
    
    // Initialize base cases
    if (memo[0] == 0) memo[0] = 1;
    if (memo[1] == 0) memo[1] = 1;
    
    // Compute values using recurrence
    for (idx_t m = 2; m <= n; m++) {
        if (memo[m] != 0) continue;
        
        memo[m] = 0;
        idx_t limit = (m + 1) / 2;
        for (idx_t k = 1; k <= limit; k++) {
            seq_t binom = a000707_binom(m, 2 * k - 1);
            seq_t prev = memo[m - 2 * k + 1];
            memo[m] += binom * prev;
        }
    }
    
    return memo[n];
}
