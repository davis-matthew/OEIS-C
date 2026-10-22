/*
 * A005208  Operator-oriented complexity of n, i.e., the minimum number of
 *          occurrences of +, *, and ^ needed to build n from a supply of
 *          ones.
 * https://oeis.org/A005208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A025280(n) - 1, minimum number of binary operations (+,
 *            *, ^) to build n from ones
 * Range:     u8 n <= 879, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;
    static idx_t memo_computed = 0;
    
    if (memo == NULL) {
        memo_size = n < 1024 ? 1024 : n * 2;
        memo = (seq_t *)malloc(memo_size * sizeof(seq_t));
        memset(memo, 0, memo_size * sizeof(seq_t));
        memo[1] = 0;
        memo_computed = 1;
    } else if (n >= memo_size) {
        idx_t old_size = memo_size;
        memo_size = memo_size * 2;
        while (n >= memo_size) memo_size *= 2;
        
        seq_t *new_memo = (seq_t *)malloc(memo_size * sizeof(seq_t));
        memcpy(new_memo, memo, old_size * sizeof(seq_t));
        memset((char *)new_memo + old_size * sizeof(seq_t), 0,
               (memo_size - old_size) * sizeof(seq_t));
        free(memo);
        memo = new_memo;
    }
    
    for (idx_t i = memo_computed + 1; i <= n; i++) {
        seq_t result = (seq_t)i - 1;
        
        for (idx_t a_val = 1; a_val <= i / 2; a_val++) {
            idx_t b_val = i - a_val;
            seq_t cost = memo[a_val] + memo[b_val] + 1;
            if (cost < result) result = cost;
        }
        
        for (idx_t a_val = 2; a_val * a_val <= i; a_val++) {
            if (i % a_val == 0) {
                idx_t b_val = i / a_val;
                seq_t cost = memo[a_val] + memo[b_val] + 1;
                if (cost < result) result = cost;
            }
        }
        
        for (idx_t a_val = 2; a_val * a_val <= i; a_val++) {
            idx_t power = a_val * a_val;
            idx_t b_val = 2;
            while (power <= i) {
                if (power == i) {
                    seq_t cost = memo[a_val] + memo[b_val] + 1;
                    if (cost < result) result = cost;
                }
                if (power > i / a_val) break;
                power *= a_val;
                b_val++;
            }
        }
        
        memo[i] = result;
    }
    
    memo_computed = n;
    return memo[n];
}
