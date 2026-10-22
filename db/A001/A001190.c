/*
 * A001190  Wedderburn-Etherington numbers: unlabeled binary rooted trees
 *          (every node has outdegree 0 or 2) with n endpoints (and 2n-1 nodes
 *          in all).
 * https://oeis.org/A001190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, core, nonn, nice, eigen
 * Formula:   a(2n-1) = sum_{k=1}^{n} a(k)*a(2n-k), a(2n) = sum_{k=1}^{n-1}
 *            a(k)*a(2n-k) + a(n)*(a(n)+1)/2, a(0)=0, a(1)=1
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 31, u64 n <= 56, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static idx_t memo_cap = 0;
    static idx_t memo_len = 0;
    
    if (memo == NULL) {
        memo_cap = 16;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        memo[0] = 0;
        memo[1] = 1;
        memo_len = 2;
    }
    
    if (n < memo_len) {
        return memo[n];
    }
    
    // Grow memo if needed
    while (memo_cap <= n) {
        memo_cap *= 2;
    }
    seq_t *new_memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
    for (idx_t i = 0; i < memo_len; i++) {
        new_memo[i] = memo[i];
    }
    free(memo);
    memo = new_memo;
    
    // Compute values from memo_len to n
    for (idx_t i = memo_len; i <= n; i++) {
        seq_t result = 0;
        if (i % 2 == 1) {
            // Odd index: a(2k+1) where k = (i-1)/2
            idx_t k = (i - 1) / 2;
            for (idx_t j = 1; j <= k; j++) {
                result = result + memo[j] * memo[i - j];
            }
        } else {
            // Even index: a(2k) where k = i/2
            idx_t k = i / 2;
            for (idx_t j = 1; j < k; j++) {
                result = result + memo[j] * memo[i - j];
            }
            seq_t ak = memo[k];
            result = result + ak * (ak + 1) / 2;
        }
        memo[i] = result;
    }
    memo_len = n + 1;
    
    return memo[n];
}
