/*
 * A000506  One half of the number of permutations of [n] such that the
 *          differences have 5 runs with the same signs.
 * https://oeis.org/A000506
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (number of permutations of [n] with exactly 5 runs of
 *            same-sign differences) / 2 A run is a maximal sequence of
 *            consecutive differences with the same sign. 5 runs means 4 sign
 *            alternations
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 6) return 0;
    
    /* DP using rank sequence representation.
       dp[i][r][s][a] = count of permutations with i elements placed, last element has rank r,
       last difference has sign s (0=ascent, 1=descent), and a alternations so far (4 = 5 runs).
    */
    
    seq_t *curr = (seq_t *)calloc(2LL * (n+1) * (n+1), sizeof(seq_t));
    seq_t *prev = (seq_t *)calloc(2LL * (n+1) * (n+1), sizeof(seq_t));
    
    #define IDX(r, s, a) (2LL * (n + 1) * (r) + (n + 1) * (s) + (a))
    
    prev[IDX(1, 0, 0)] = 1;
    
    for (idx_t i = 1; i < n; i++) {
        memset(curr, 0, 2LL * (n+1) * (n+1) * sizeof(seq_t));
        
        for (idx_t r_prev = 1; r_prev <= i; r_prev++) {
            for (int s_prev = 0; s_prev < 2; s_prev++) {
                for (idx_t a_prev = 0; a_prev < i; a_prev++) {
                    seq_t val = prev[IDX(r_prev, s_prev, a_prev)];
                    if (val == 0) continue;
                    
                    for (idx_t r = 1; r <= i + 1; r++) {
                        int s = (r > r_prev) ? 0 : 1;
                        idx_t a_new;
                        
                        if (i == 1) {
                            a_new = 0;
                        } else if (s == s_prev) {
                            a_new = a_prev;
                        } else {
                            a_new = a_prev + 1;
                        }
                        
                        if (a_new < i) {
                            curr[IDX(r, s, a_new)] += val;
                        }
                    }
                }
            }
        }
        
        seq_t *temp = curr;
        curr = prev;
        prev = temp;
    }
    
    seq_t result = 0;
    for (idx_t r = 1; r <= n; r++) {
        for (int s = 0; s < 2; s++) {
            result += prev[IDX(r, s, 4)];
        }
    }
    
    free(curr);
    free(prev);
    #undef IDX
    
    return result / 2;
}
