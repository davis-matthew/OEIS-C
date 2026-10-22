/*
 * A002020  a(n+1) = a(n) - n*(n-1)*a(n-1), with a(n) = 1 for n <= 3.
 * https://oeis.org/A002020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n+1) = a(n) - n*(n-1)*a(n-1), a(0)=a(1)=a(2)=a(3)=1
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 14, i64 n <= 22, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;
static idx_t computed_up_to = 0;

seq_t a(idx_t n) {
    if (n <= 3) {
        return (seq_t)1;
    }
    
    if (memo == NULL) {
        memo_len = (n < 1000) ? 1024 : n + 100;
        memo = (seq_t *)malloc(memo_len * sizeof(seq_t));
        memo[0] = 1;
        memo[1] = 1;
        memo[2] = 1;
        memo[3] = 1;
        computed_up_to = 3;
    }
    
    while (n >= memo_len) {
        memo_len *= 2;
        memo = (seq_t *)realloc(memo, memo_len * sizeof(seq_t));
    }
    
    while (computed_up_to < n) {
        idx_t i = computed_up_to + 1;
        seq_t prev = memo[i - 1];
        seq_t prev_prev = memo[i - 2];
        seq_t factor = (seq_t)((i - 1) * (i - 2));
        memo[i] = prev - factor * prev_prev;
        computed_up_to = i;
    }
    
    return memo[n];
}
