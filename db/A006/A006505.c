/*
 * A006505  Number of partitions of an n-set into boxes of size >2.
 * https://oeis.org/A006505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(0) = 1; a(n) = Sum_{k=0..n-3} binomial(n-1,k+2) *
 *            a(n-k-3)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 17, u64 n <= 28, u128 n <= 45,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static bool *computed = NULL;
    static idx_t memo_len = 0;
    
    if (memo == NULL) {
        memo_len = 1024;
        memo = (seq_t *)calloc(memo_len, sizeof(seq_t));
        computed = (bool *)calloc(memo_len, sizeof(bool));
        memo[0] = 1;
        computed[0] = true;
    }
    
    if (n >= memo_len) {
        idx_t old_len = memo_len;
        while (memo_len <= n) memo_len *= 2;
        seq_t *new_memo = (seq_t *)calloc(memo_len, sizeof(seq_t));
        bool *new_computed = (bool *)calloc(memo_len, sizeof(bool));
        memcpy(new_memo, memo, old_len * sizeof(seq_t));
        memcpy(new_computed, computed, old_len * sizeof(bool));
        free(memo);
        free(computed);
        memo = new_memo;
        computed = new_computed;
    }
    
    if (computed[n]) {
        return memo[n];
    }
    
    seq_t result = 0;
    
    if (n >= 3) {
        for (idx_t k = 0; k <= n - 3; k++) {
            result += binom(n - 1, k + 2) * a(n - k - 3);
        }
    }
    
    memo[n] = result;
    computed[n] = true;
    return result;
}
