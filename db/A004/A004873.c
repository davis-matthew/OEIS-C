/*
 * A004873  Numbers that are the sum of at most 11 positive 7th powers.
 * https://oeis.org/A004873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers representable as sum of at most 11 positive 7th powers;
 *            computed via unbounded knapsack DP
 * Range:     u8 n <= 12, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   a000272_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a000272_ipow.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

static void build_memo(idx_t needed) {
    if (memo_len >= needed) return;
    
    uint64_t target = (uint64_t)needed * 50;
    
    while (target <= (1ULL << 31)) {
        seq_t *dp = (seq_t *)malloc((size_t)(target + 1) * sizeof(seq_t));
        
        for (uint64_t i = 0; i <= target; i++) {
            dp[i] = (seq_t)12;
        }
        dp[0] = (seq_t)0;
        
        for (idx_t base = 1; base <= 1000; base++) {
            seq_t power = a000272_ipow((seq_t)base, (idx_t)7);
            uint64_t power_u64 = (uint64_t)power;
            
            if (power_u64 > target) break;
            
            for (uint64_t val = power_u64; val <= target; val++) {
                seq_t prev = dp[val - power_u64];
                if (prev < (seq_t)11) {
                    seq_t new_count = prev + (seq_t)1;
                    if (new_count < dp[val]) {
                        dp[val] = new_count;
                    }
                }
            }
        }
        
        uint64_t count = 0;
        for (uint64_t i = 0; i <= target; i++) {
            if (dp[i] <= (seq_t)11) count++;
        }
        
        if ((idx_t)count >= needed) {
            memo = (seq_t *)realloc(memo, (size_t)count * sizeof(seq_t));
            uint64_t idx = 0;
            for (uint64_t i = 0; i <= target && idx < count; i++) {
                if (dp[i] <= (seq_t)11) {
                    memo[idx++] = (seq_t)i;
                }
            }
            memo_len = (idx_t)count;
            free(dp);
            return;
        }
        
        free(dp);
        target *= 2;
    }
}

seq_t a(idx_t n) {
    build_memo(n);
    return memo[n - 1];
}
