/*
 * A001277  Number of permutations of length n by rises.
 * https://oeis.org/A001277
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) - (n-1)*a(n-3), a(2) = 1, a(3) = 3, a(4) = 12
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *memo = NULL;
static idx_t memo_capacity = 0;
static idx_t computed_up_to = 1;

seq_t a(idx_t n) {
    if (n < 2) return 0;
    if (n == 2) return 1;
    if (n == 3) return 3;
    if (n == 4) return 12;
    
    if (memo == NULL) {
        memo_capacity = (n + 1000 < 100000) ? 100000 : n + 10000;
        memo = (seq_t *)malloc(memo_capacity * sizeof(seq_t));
        if (memo == NULL) return 0;
        memset(memo, 0, memo_capacity * sizeof(seq_t));
        memo[2] = 1;
        memo[3] = 3;
        memo[4] = 12;
        computed_up_to = 4;
    }
    
    if (n <= computed_up_to) {
        return memo[n];
    }
    
    if (n >= memo_capacity) {
        idx_t new_capacity = n + 10000;
        seq_t *new_memo = (seq_t *)malloc(new_capacity * sizeof(seq_t));
        if (new_memo == NULL) return 0;
        memcpy(new_memo, memo, memo_capacity * sizeof(seq_t));
        memset(new_memo + memo_capacity, 0, (new_capacity - memo_capacity) * sizeof(seq_t));
        free(memo);
        memo = new_memo;
        memo_capacity = new_capacity;
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        memo[i] = (seq_t)i * memo[i - 1] - (seq_t)(i - 1) * memo[i - 3];
    }
    
    computed_up_to = n;
    return memo[n];
}
