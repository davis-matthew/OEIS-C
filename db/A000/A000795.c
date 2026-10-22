/*
 * A000795  Salié numbers: expansion of cosh x / cos x = Sum_{n >= 0}
 *          a(n)*x^(2n)/(2n)!.
 * https://oeis.org/A000795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1 - Sum_{k=1..n} (-1)^k * binomial(2n, 2k) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   a000707_binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a000707_binom.h"


seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;
    
    // Expand memo if necessary
    if (n >= memo_size) {
        idx_t new_size = (n + 1) * 2;
        seq_t *new_memo = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (memo != NULL) {
            for (idx_t i = 0; i < memo_size; i++) {
                new_memo[i] = memo[i];
            }
            free(memo);
        }
        // Zero out new memory
        for (idx_t i = memo_size; i < new_size; i++) {
            new_memo[i] = 0;
        }
        memo = new_memo;
        memo_size = new_size;
    }
    
    // Return cached value if available
    if (memo[n] != 0) {
        return memo[n];
    }
    
    // Compute a(n) using the recurrence
    seq_t result = 1;
    for (idx_t k = 1; k <= n; k++) {
        seq_t binom = a000707_binom(2*n, 2*k);
        seq_t prev = a(n - k);
        
        if (k % 2 == 1) {
            result += binom * prev;
        } else {
            result -= binom * prev;
        }
    }
    
    memo[n] = result;
    return result;
}
