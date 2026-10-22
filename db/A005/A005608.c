/*
 * A005608  Number of Boolean functions realized by cascades of n gates.
 * https://oeis.org/A005608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = N(n,3,2) where N(n,s,r) = Sum_{i=1..s} S(n,i,s,r) with
 *            S(1,i,s,r) = binomial(s,i) * Sum_{k=0..i} (-1)^k * binomial(i,k)
 *            * (i-k)^(s*r) and S(n,i,s,r) = Sum_{j=1..s}
 *            (S(n-1,j,s,r)/binomial(s,j)) * binomial(s,i) * Sum_{k=0..i-1}
 *            (-1)^k * binomial(i,k) * binomial((i-k)^r,j)
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"


seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    seq_t S_prev[4];
    seq_t S_curr[4];
    
    // Base case: S(1, i, 3, 2)
    for (idx_t i = 1; i <= 3; i++) {
        seq_t sum = 0;
        for (idx_t k = 0; k <= i; k++) {
            seq_t binom_i_k = binom(i, k);
            seq_t base = (seq_t)(i - k);
            seq_t base_sq = base * base;
            seq_t base_pow6 = base_sq * base_sq * base_sq;
            
            seq_t term = binom_i_k * base_pow6;
            if (k % 2 == 0) {
                sum += term;
            } else {
                sum -= term;
            }
        }
        S_prev[i] = binom(3, i) * sum;
    }
    
    if (n == 1) {
        return S_prev[1] + S_prev[2] + S_prev[3];
    }
    
    // Recursive case
    for (idx_t m = 2; m <= n; m++) {
        for (idx_t i = 1; i <= 3; i++) {
            seq_t s_sum = 0;
            
            for (idx_t j = 1; j <= 3; j++) {
                seq_t binom_3_j = binom(3, j);
                seq_t quotient = S_prev[j] / binom_3_j;
                seq_t binom_3_i = binom(3, i);
                
                seq_t inner_sum = 0;
                for (idx_t k = 0; k < i; k++) {
                    seq_t binom_i_k = binom(i, k);
                    seq_t base = (seq_t)(i - k);
                    seq_t base_sq = base * base;
                    seq_t binom_base_sq_j = binom((idx_t)base_sq, j);
                    
                    seq_t term = binom_i_k * binom_base_sq_j;
                    if (k % 2 == 0) {
                        inner_sum += term;
                    } else {
                        inner_sum -= term;
                    }
                }
                
                s_sum += quotient * binom_3_i * inner_sum;
            }
            
            S_curr[i] = s_sum;
        }
        
        S_prev[1] = S_curr[1];
        S_prev[2] = S_curr[2];
        S_prev[3] = S_curr[3];
    }
    
    return S_prev[1] + S_prev[2] + S_prev[3];
}
