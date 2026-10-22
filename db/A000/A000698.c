/*
 * A000698  A problem of configurations: a(0) = 1; for n>0, a(n) = (2n-1)!! -
 *          Sum_{k=1..n-1} (2k-1)!! a(n-k). Also the number of shellings of an
 *          n-cube, divided by 2^n n!.
 * https://oeis.org/A000698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (2n-1)!! - Sum_{k=1..n-1} (2k-1)!! a(n-k), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *memo = NULL;
static idx_t memo_capacity = 0;
static idx_t memo_computed = 0;

static seq_t *df_memo = NULL;
static idx_t df_memo_capacity = 0;
static idx_t df_memo_computed = 0;

static seq_t double_factorial(idx_t n) {
    if (n >= df_memo_computed) {
        if (df_memo == NULL) {
            df_memo_capacity = 1024;
            df_memo = (seq_t *)malloc(df_memo_capacity * sizeof(seq_t));
            df_memo[0] = 1;
            df_memo_computed = 1;
        }
        
        while (n >= df_memo_capacity) {
            df_memo_capacity *= 2;
            seq_t *new_df_memo = (seq_t *)malloc(df_memo_capacity * sizeof(seq_t));
            memcpy(new_df_memo, df_memo, df_memo_computed * sizeof(seq_t));
            free(df_memo);
            df_memo = new_df_memo;
        }
        
        for (idx_t i = df_memo_computed; i <= n; i++) {
            df_memo[i] = df_memo[i-1] * (2*i - 1);
        }
        
        df_memo_computed = n + 1;
    }
    
    return df_memo[n];
}

seq_t a(idx_t n) {
    if (memo == NULL) {
        memo_capacity = 1024;
        memo = (seq_t *)malloc(memo_capacity * sizeof(seq_t));
        memo[0] = 1;
        memo_computed = 1;
    }
    
    if (n < memo_computed) {
        return memo[n];
    }
    
    while (n >= memo_capacity) {
        memo_capacity *= 2;
        seq_t *new_memo = (seq_t *)malloc(memo_capacity * sizeof(seq_t));
        memcpy(new_memo, memo, memo_computed * sizeof(seq_t));
        free(memo);
        memo = new_memo;
    }
    
    for (idx_t i = memo_computed; i <= n; i++) {
        seq_t result = double_factorial(i);
        for (idx_t k = 1; k < i; k++) {
            seq_t term = double_factorial(k) * memo[i - k];
            result -= term;
        }
        memo[i] = result;
    }
    
    memo_computed = n + 1;
    return memo[n];
}
