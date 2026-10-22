/*
 * A006669  Exponentiation of g.f. for Pell numbers.
 * https://oeis.org/A006669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(-1) = 1, a(n) = Sum_{k=0..n} binomial(n, k) * A000129(k) *
 *            a(n-k-1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binom, pell
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/binom.h"
#include "../utils/pell.h"

seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;
    
    if (n < memo_size) {
        return memo[n];
    }
    
    if (memo == NULL) {
        memo = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    } else {
        memo = (seq_t *)realloc(memo, (n + 1) * sizeof(seq_t));
    }
    
    for (idx_t i = memo_size; i <= n; i++) {
        seq_t sum = 0;
        for (idx_t k = 0; k <= i; k++) {
            seq_t pk = pell(k);
            seq_t binom_ik = binom((idx_t)i, (idx_t)k);
            
            seq_t a_prev;
            if (i == k) {
                a_prev = 1;
            } else {
                a_prev = memo[i - k - 1];
            }
            
            sum = sum + binom_ik * pk * a_prev;
        }
        memo[i] = sum;
    }
    
    memo_size = n + 1;
    return memo[n];
}
