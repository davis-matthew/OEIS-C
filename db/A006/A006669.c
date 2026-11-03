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
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

static seq_t pell(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    seq_t p0 = 0, p1 = 1;
    for (idx_t i = 2; i <= n; i++) {
        seq_t p2 = (seq_t)2 * p1 + p0;
        p0 = p1;
        p1 = p2;
    }
    return p1;
}

seq_t a(idx_t n) {
    static seq_t *memo = NULL;
    static idx_t memo_size = 0;
    
    if (n < memo_size) {
        return memo[n];
    }
    
    if (memo == NULL) {
        memo = mem_new(seq_t, (n + 1));
    } else {
        memo = mem_resize(seq_t, memo, (n + 1));
    }
    
    for (idx_t i = memo_size; i <= n; i++) {
        seq_t sum = 0;
        for (idx_t k = 0; k <= i; k++) {
            seq_t pk = pell(k);
            seq_t binom_ik = binomial((idx_t)i, (idx_t)k);
            
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
