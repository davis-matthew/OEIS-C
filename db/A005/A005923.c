/*
 * A005923  From solution to a difference equation.
 * https://oeis.org/A005923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n} (-1)^(k + 1) * binomial(n,k) *
 *            (2^k + 1) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-03)
 * Helpers:   binomial, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static seq_t *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (memo == NULL) {
        memo = alloc_zero(1);
        memo[0] = 1;
        memo_len = 1;
    }
    
    if (n < memo_len) {
        return memo[n];
    }
    
    memo = grow_zero(memo, memo_len, n + 1);
    
    for (idx_t i = memo_len; i <= n; i++) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; k++) {
            seq_t binom_val = binomial(i, k);
            seq_t power_term = pow2(k) + 1;
            seq_t prev_val = memo[i - k];
            
            if (k % 2 == 1) {
                sum = sum + binom_val * power_term * prev_val;
            } else {
                sum = sum - binom_val * power_term * prev_val;
            }
        }
        memo[i] = sum;
    }
    
    memo_len = n + 1;
    return memo[n];
}
