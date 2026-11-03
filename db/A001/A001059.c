/*
 * A001059  Number of doubly labeled heap-ordered trees.
 * https://oeis.org/A001059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n, k)^2*a(k)*a(n-k-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 96 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

static seq_t *memo = NULL;
static bool *computed = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return 1;
    
    if (memo == NULL || n >= memo_len) {
        idx_t new_len = n + 1;
        if (new_len < 16) new_len = 16;
        else new_len = new_len * 2;
        
        seq_t *new_memo = mem_new(seq_t, new_len);
        bool *new_computed = (bool *)malloc(new_len * sizeof(bool));
        
        if (memo != NULL) {
            for (idx_t i = 0; i < memo_len; i++) {
                new_memo[i] = memo[i];
                new_computed[i] = computed[i];
            }
            mem_free(memo);
            free(computed);
        }
        
        for (idx_t i = memo_len; i < new_len; i++) {
            new_computed[i] = false;
        }
        
        memo = new_memo;
        computed = new_computed;
        memo_len = new_len;
    }
    
    if (computed[n]) {
        return memo[n];
    }
    
    seq_t result = 0;
    for (idx_t k = 0; k < n; k++) {
        seq_t binom_nk = binomial(n, k);
        seq_t binom_sq = binom_nk * binom_nk;
        seq_t ak = a(k);
        seq_t an_k_1 = a(n - k - 1);
        result = result + binom_sq * ak * an_k_1;
    }
    
    memo[n] = result;
    computed[n] = true;
    return result;
}
