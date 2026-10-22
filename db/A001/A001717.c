/*
 * A001717  Generalized Stirling numbers, [n+6,6]_4.
 * https://oeis.org/A001717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+3)!/6 + (2*n+9)*a(n-1) - (n+4)^2*a(n-2) for n >= 2,
 *            with a(0)=1, a(1)=15
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *memo = NULL;
static seq_t *fact = NULL;
static idx_t memo_cap = 0;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 15;
    
    if (n >= memo_cap) {
        idx_t new_cap = (n + 1) * 2;
        seq_t *new_memo = (seq_t *)malloc(new_cap * sizeof(seq_t));
        seq_t *new_fact = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (new_memo == NULL || new_fact == NULL) {
            free(new_memo);
            free(new_fact);
            return 0;
        }
        
        if (memo != NULL) {
            for (idx_t i = 0; i < memo_len; i++) {
                new_memo[i] = memo[i];
                new_fact[i] = fact[i];
            }
            free(memo);
            free(fact);
        }
        
        memo = new_memo;
        fact = new_fact;
        memo_cap = new_cap;
    }
    
    if (memo_len == 0) {
        memo[0] = 1;
        memo[1] = 15;
        fact[0] = 1;
        fact[1] = 4;
        memo_len = 2;
    }
    
    for (idx_t i = memo_len; i <= n; i++) {
        fact[i] = fact[i-1] * ((seq_t)(i+3));
        
        seq_t term1 = fact[i];
        seq_t term2 = ((seq_t)(2*i + 9)) * memo[i-1];
        seq_t coeff = (seq_t)(i+4);
        seq_t term3 = coeff * coeff * memo[i-2];
        
        memo[i] = term1 + term2 - term3;
        memo_len = i + 1;
    }
    
    return memo[n];
}
