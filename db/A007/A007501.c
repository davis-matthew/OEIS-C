/*
 * A007501  a(0) = 2; for n >= 0, a(n+1) = a(n)*(a(n)+1)/2.
 * https://oeis.org/A007501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2; for n >= 0, a(n+1) = a(n)*(a(n)+1)/2
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_len = 0;
static idx_t computed_up_to = 0;
static int initialized = 0;

seq_t a(idx_t n) {
    if (memo_len <= n) {
        idx_t new_len = n + 1;
        seq_t *new_memo = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (memo != NULL) {
            for (idx_t i = 0; i < memo_len; i++) {
                new_memo[i] = memo[i];
            }
            free(memo);
        }
        memo = new_memo;
        memo_len = new_len;
    }
    
    if (!initialized) {
        memo[0] = 2;
        computed_up_to = 0;
        initialized = 1;
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        seq_t prev = memo[i - 1];
        memo[i] = prev * (prev + 1) / 2;
    }
    
    if (n > computed_up_to) {
        computed_up_to = n;
    }
    
    return memo[n];
}
