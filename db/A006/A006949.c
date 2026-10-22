/*
 * A006949  A well-behaved cousin of the Hofstadter sequence: a(n) = a(n - 1 -
 *          a(n-1)) + a(n - 2 - a(n-2)) for n > 2 with a(0) = a(1) = a(2) = 1.
 * https://oeis.org/A006949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n - 1 - a(n-1)) + a(n - 2 - a(n-2)) for n > 2 with a(0)
 *            = a(1) = a(2) = 1
 * Range:     u8 n <= 510, u16 n <= 9215, u32 n <= 9215, u64 n <= 9215,
 *            u128 n <= 9215, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static idx_t memo_cap = 0;
    static idx_t computed_up_to = 0;
    
    if (memo == NULL) {
        memo_cap = 64;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        memo[0] = 1;
        memo[1] = 1;
        memo[2] = 1;
        computed_up_to = 2;
    }
    
    if (n <= computed_up_to) {
        return memo[n];
    }
    
    // Extend memo if needed
    while (n >= memo_cap) {
        memo_cap *= 2;
        memo = (seq_t *)realloc(memo, memo_cap * sizeof(seq_t));
    }
    
    // Compute values from computed_up_to + 1 to n
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        idx_t idx1 = i - 1 - (idx_t)memo[i - 1];
        idx_t idx2 = i - 2 - (idx_t)memo[i - 2];
        memo[i] = memo[idx1] + memo[idx2];
    }
    
    computed_up_to = n;
    return memo[n];
}
