/*
 * A006161  a(n)=a(a(n-1)-1)+a(n+1-a(n-1)).
 * https://oeis.org/A006161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(a(n-1)-1) + a(n+1-a(n-1)), a(1) = 1, a(2) = 1, a(3) = 2
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *memo = NULL;
static idx_t memo_capacity = 0;
static idx_t computed_up_to = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    if (n <= 3) {
        if (n == 1) return 1;
        if (n == 2) return 1;
        return 2;
    }
    
    if (memo == NULL) {
        memo_capacity = (n < 1024) ? 1024 : n + 256;
        memo = (seq_t *)calloc(memo_capacity, sizeof(seq_t));
        memo[1] = 1;
        memo[2] = 1;
        memo[3] = 2;
        computed_up_to = 3;
    }
    
    if (n >= memo_capacity) {
        idx_t new_capacity = n + 256;
        seq_t *new_memo = (seq_t *)calloc(new_capacity, sizeof(seq_t));
        memcpy(new_memo, memo, memo_capacity * sizeof(seq_t));
        free(memo);
        memo = new_memo;
        memo_capacity = new_capacity;
    }
    
    if (n > computed_up_to) {
        idx_t i;
        for (i = computed_up_to + 1; i <= n; i++) {
            seq_t prev = memo[i - 1];
            idx_t idx1 = (idx_t)prev - 1;
            idx_t idx2 = i + 1 - (idx_t)prev;
            
            seq_t result = 0;
            if (idx1 >= 1) result += memo[idx1];
            if (idx2 >= 1) result += memo[idx2];
            
            memo[i] = result;
        }
        computed_up_to = n;
    }
    
    return memo[n];
}
