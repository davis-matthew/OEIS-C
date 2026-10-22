/*
 * A006358  Number of antichains in the product of a fence poset with n
 *          elements and a chain with 4 elements; also number of paths with n
 *          turns when light is reflected from 5 glass plates.
 * https://oeis.org/A006358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - 4*a(n-3) - a(n-4) + a(n-5); a(0)=1,
 *            a(1)=5, a(2)=15, a(3)=55, a(4)=190
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


static seq_t *memo = NULL;
static idx_t memo_len = 0;
static idx_t max_computed = 0;

seq_t a(idx_t n) {
    seq_t base[5] = {1, 5, 15, 55, 190};
    if (n < 5) return base[n];
    
    if (memo_len <= n) {
        idx_t new_len = n + 1;
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (memo != NULL) {
            memcpy(new_memo, memo, memo_len * sizeof(seq_t));
            free(memo);
        }
        memo = new_memo;
        memo_len = new_len;
    }
    
    if (max_computed == 0) {
        for (idx_t i = 0; i < 5; i++) {
            memo[i] = base[i];
        }
        max_computed = 5;
    }
    
    if (n < max_computed) {
        return memo[n];
    }
    
    for (idx_t i = max_computed; i <= n; i++) {
        memo[i] = 3*memo[i-1] + 3*memo[i-2] - 4*memo[i-3] - memo[i-4] + memo[i-5];
    }
    max_computed = n + 1;
    
    return memo[n];
}
