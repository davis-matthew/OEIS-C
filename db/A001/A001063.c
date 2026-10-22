/*
 * A001063  E.g.f. satisfies A'(x) = A(x/(1-x)).
 * https://oeis.org/A001063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(n+1) = Sum_{k=0..n} n!/k! * binomial(n-1, k-1) * a(k), a(0) =
 *            1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static seq_t *memo = NULL;
static bool *computed = NULL;
static idx_t memo_size = 0;

static seq_t binomial(int64_t n, int64_t k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    
    if (k > n - k) k = n - k;
    
    seq_t result = 1;
    for (int64_t i = 0; i < k; i++) {
        result *= (seq_t)(n - i);
        result /= (seq_t)(i + 1);
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return 1;
    
    if (memo == NULL) {
        memo_size = 16;
        memo = (seq_t *)malloc(memo_size * sizeof(seq_t));
        computed = (bool *)malloc(memo_size * sizeof(bool));
        memset(memo, 0, memo_size * sizeof(seq_t));
        memset(computed, 0, memo_size * sizeof(bool));
        memo[0] = 1;
        memo[1] = 1;
        computed[0] = true;
        computed[1] = true;
    }
    
    while ((idx_t)memo_size <= n) {
        idx_t new_size = memo_size * 2;
        seq_t *new_memo = (seq_t *)malloc(new_size * sizeof(seq_t));
        bool *new_computed = (bool *)malloc(new_size * sizeof(bool));
        memset(new_memo, 0, new_size * sizeof(seq_t));
        memset(new_computed, 0, new_size * sizeof(bool));
        memcpy(new_memo, memo, memo_size * sizeof(seq_t));
        memcpy(new_computed, computed, memo_size * sizeof(bool));
        free(memo);
        free(computed);
        memo = new_memo;
        computed = new_computed;
        memo_size = new_size;
    }
    
    if (computed[n]) return memo[n];
    
    idx_t m = n - 1;
    seq_t result = 0;
    
    for (idx_t k = 0; k < n; k++) {
        seq_t fact_ratio = 1;
        for (idx_t i = k + 1; i <= m; i++) {
            fact_ratio *= (seq_t)i;
        }
        
        seq_t binom = binomial((int64_t)m - 1, (int64_t)k - 1);
        seq_t ak = a(k);
        
        result += fact_ratio * binom * ak;
    }
    
    memo[n] = result;
    computed[n] = true;
    return result;
}
