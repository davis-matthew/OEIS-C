/*
 * A006492  Generalized Lucas numbers.
 * https://oeis.org/A006492
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 8*a(n-3) + 5*a(n-4) + 8*a(n-5) -
 *            2*a(n-6) - 4*a(n-7) - a(n-8) for n >= 8, with a(3)=1, a(4)=0,
 *            a(5)=5, a(6)=6, a(7)=21, a(n)=0 for n<3
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 37, u64 n <= 79, u128 n <= 167,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static struct {
    seq_t *values;
    idx_t capacity;
    idx_t computed_up_to;
} memo = {NULL, 0, 7};

seq_t a(idx_t n) {
    if (n < 3) return 0;
    
    if (n <= 7) {
        seq_t bases[8] = {0, 0, 0, 1, 0, 5, 6, 21};
        return bases[n];
    }
    
    if (memo.capacity == 0) {
        memo.capacity = (n > 1000) ? n + 1000 : 2000;
        memo.values = (seq_t *)malloc(memo.capacity * sizeof(seq_t));
        if (!memo.values) return 0;
        
        seq_t bases[8] = {0, 0, 0, 1, 0, 5, 6, 21};
        for (idx_t i = 0; i < 8; i++) {
            memo.values[i] = bases[i];
        }
    } else if (n >= memo.capacity) {
        memo.capacity = n + 1000;
        seq_t *new_values = (seq_t *)realloc(memo.values, memo.capacity * sizeof(seq_t));
        if (!new_values) return 0;
        memo.values = new_values;
    }
    
    if (n <= memo.computed_up_to) {
        return memo.values[n];
    }
    
    for (idx_t i = memo.computed_up_to + 1; i <= n; i++) {
        seq_t val = 4*memo.values[i-1] - 2*memo.values[i-2] - 8*memo.values[i-3] + 5*memo.values[i-4] + 8*memo.values[i-5] - 2*memo.values[i-6] - 4*memo.values[i-7] - memo.values[i-8];
        memo.values[i] = val;
    }
    
    memo.computed_up_to = n;
    return memo.values[n];
}
