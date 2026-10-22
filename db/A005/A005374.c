/*
 * A005374  Hofstadter H-sequence: a(n) = n - a(a(a(n-1))).
 * https://oeis.org/A005374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n - a(a(a(n-1))), a(0) = 0
 * Range:     u8 n <= 374, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *memo = NULL;
static idx_t memo_size = 0;
static idx_t computed_up_to = 0;
static bool initialized = false;

seq_t a(idx_t n) {
    if (!initialized) {
        memo_size = 1024;
        memo = (seq_t *)malloc(memo_size * sizeof(seq_t));
        if (memo == NULL) return (seq_t)0;
        memo[0] = (seq_t)0;
        computed_up_to = 0;
        initialized = true;
    }
    
    if (n == 0) return memo[0];
    
    if (n >= memo_size) {
        idx_t new_size = n + 1024;
        seq_t *new_memo = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (new_memo == NULL) return (seq_t)0;
        
        for (idx_t i = 0; i < memo_size; i++) {
            new_memo[i] = memo[i];
        }
        free(memo);
        
        memo = new_memo;
        memo_size = new_size;
    }
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        seq_t a_i_minus_1 = memo[i - 1];
        seq_t a_a_i_minus_1 = memo[(long long)a_i_minus_1];
        seq_t a_a_a_i_minus_1 = memo[(long long)a_a_i_minus_1];
        memo[i] = (seq_t)i - a_a_a_i_minus_1;
    }
    
    computed_up_to = (n > computed_up_to) ? n : computed_up_to;
    
    return memo[n];
}
