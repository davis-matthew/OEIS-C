/*
 * A003403  G.f.: (1 + x^3 + x^4 + ... + x^12 + x^15)/Product_{i=1..10} (1 -
 *          x^i).
 * https://oeis.org/A003403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: (1 + x^3 + x^4 + ... + x^12 + x^15)/Product_{i=1..10} (1 -
 *            x^i)
 * Range:     u8 n <= 13, u16 n <= 38, u32 n <= 178, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;
    static idx_t computed_up_to = 0;
    static int computed_any = 0;
    static seq_t denom[56];
    static int denom_computed = 0;
    
    // Compute denominator coefficients once
    if (!denom_computed) {
        for (int i = 0; i < 56; i++) {
            denom[i] = 0;
        }
        denom[0] = 1;
        
        // Multiply by (1 - x^i) for i = 1 to 10
        for (int i = 1; i <= 10; i++) {
            for (int j = 55; j >= i; j--) {
                denom[j] -= denom[j - i];
            }
        }
        denom_computed = 1;
    }
    
    // Quick return if already computed
    if (computed_any && n <= computed_up_to) {
        return memo[n];
    }
    
    // Allocate or extend memo
    if (memo_size == 0 || n >= memo_size) {
        idx_t new_size = (n + 256) & ~(idx_t)255;
        seq_t *new_memo = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (memo != NULL) {
            for (idx_t i = 0; i <= computed_up_to; i++) {
                new_memo[i] = memo[i];
            }
            free(memo);
        }
        memo = new_memo;
        memo_size = new_size;
    }
    
    // Compute missing values
    idx_t start = computed_any ? (computed_up_to + 1) : 0;
    for (idx_t i = start; i <= n; i++) {
        seq_t numer_val = 0;
        if (i == 0 || (i >= 3 && i <= 12) || i == 15) {
            numer_val = 1;
        }
        
        seq_t result = numer_val;
        for (int j = 1; j <= 55 && j <= (int)i; j++) {
            if (denom[j] != 0) {
                result -= memo[i - (idx_t)j] * denom[j];
            }
        }
        memo[i] = result;
    }
    
    computed_up_to = n;
    computed_any = 1;
    
    return memo[n];
}
