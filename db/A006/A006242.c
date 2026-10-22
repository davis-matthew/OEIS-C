/*
 * A006242  Extracting a square root.
 * https://oeis.org/A006242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 10, a(n) = a(n-1)^3 - 3*a(n-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static idx_t memo_capacity = 0;
static idx_t last_computed = 0;

seq_t a(idx_t n) {
    // Ensure memo is large enough
    if (n > memo_capacity) {
        idx_t new_capacity = n;
        if (new_capacity < 1000) new_capacity = 1000;
        
        seq_t *new_memo = (seq_t *)malloc(sizeof(seq_t) * (new_capacity + 1));
        if (memo != NULL) {
            for (idx_t i = 0; i <= last_computed; i++) {
                new_memo[i] = memo[i];
            }
            free(memo);
        } else {
            for (idx_t i = 0; i <= new_capacity; i++) {
                new_memo[i] = 0;
            }
        }
        memo = new_memo;
        memo_capacity = new_capacity;
    }
    
    // If already computed, return it
    if (n <= last_computed) {
        return memo[n];
    }
    
    // Compute from last computed value up to n
    seq_t current;
    idx_t start;
    
    if (last_computed == 0) {
        current = (seq_t)10;
        memo[1] = current;
        last_computed = 1;
        start = 2;
    } else {
        current = memo[last_computed];
        start = last_computed + 1;
    }
    
    for (idx_t i = start; i <= n; i++) {
        current = current * current * current - (seq_t)3 * current;
        memo[i] = current;
        last_computed = i;
    }
    
    return memo[n];
}
