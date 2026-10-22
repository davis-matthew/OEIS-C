/*
 * A006191  Number of paths on square lattice.
 * https://oeis.org/A006191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) + 2*a(n-3) + a(n-4) for n > 4, with
 *            a(1) = 1, a(2) = 2, a(3) = 5, a(4) = 16
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    // Allocate or grow memo array
    if (n >= memo_len) {
        idx_t new_len = (memo_len == 0) ? 16 : memo_len;
        while (new_len <= n) new_len *= 2;
        
        seq_t *new_memo = (seq_t *)calloc(new_len + 1, sizeof(seq_t));
        if (memo != NULL) {
            memcpy(new_memo, memo, (memo_len + 1) * sizeof(seq_t));
            free(memo);
        }
        memo = new_memo;
        memo_len = new_len;
    }
    
    // Return if already computed
    if (memo[n] != 0) return memo[n];
    
    // Base cases
    if (n == 1) return memo[1] = 1;
    if (n == 2) return memo[2] = 2;
    if (n == 3) return memo[3] = 5;
    if (n == 4) return memo[4] = 16;
    
    // Ensure base cases are computed
    if (memo[1] == 0) memo[1] = 1;
    if (memo[2] == 0) memo[2] = 2;
    if (memo[3] == 0) memo[3] = 5;
    if (memo[4] == 0) memo[4] = 16;
    
    // Compute up to n
    for (idx_t i = 5; i <= n; i++) {
        if (memo[i] == 0) {
            memo[i] = 4*memo[i-1] - 3*memo[i-2] + 2*memo[i-3] + memo[i-4];
        }
    }
    
    return memo[n];
}
