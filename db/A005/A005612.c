/*
 * A005612  Number of Boolean functions of n variables that are variously
 *          called "unate cascades" or "1-decision list functions" or
 *          "read-once threshold functions".
 * https://oeis.org/A005612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2; for n > 1, a(n) = 2^(n+1) * (P_n - n*P_{n-1}) where
 *            P_n is the n-th Fubini number (A000670) computed as sum of
 *            Stirling numbers times factorials
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 300 (2026-11-03)
 * Helpers:   mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/mem.h"

static void compute_fubini_pair(idx_t n, seq_t *P_n, seq_t *P_n_minus_1) {
    if (n == 1) {
        *P_n = 1;
        *P_n_minus_1 = 1;
        return;
    }
    
    seq_t *prev = mem_new(seq_t, n + 1);
    seq_t *curr = mem_new(seq_t, n + 1);
    
    prev[0] = 1;
    
    seq_t temp_P_n_minus_1 = 1;
    seq_t temp_P_n = 0;
    
    for (idx_t i = 1; i <= n; i++) {
        curr[0] = 0;
        for (idx_t j = 1; j <= i && j <= n; j++) {
            curr[j] = (seq_t)j * prev[j] + prev[j-1];
        }
        
        if (i == n - 1) {
            temp_P_n_minus_1 = 0;
            seq_t factorial = 1;
            for (idx_t j = 0; j <= n - 1; j++) {
                if (j > 0) factorial *= (seq_t)j;
                temp_P_n_minus_1 += curr[j] * factorial;
            }
        }
        
        seq_t *temp = prev;
        prev = curr;
        curr = temp;
    }
    
    {
        seq_t factorial = 1;
        for (idx_t j = 0; j <= n; j++) {
            if (j > 0) factorial *= (seq_t)j;
            temp_P_n += prev[j] * factorial;
        }
    }
    
    *P_n = temp_P_n;
    *P_n_minus_1 = temp_P_n_minus_1;
    
    mem_free(prev);
    mem_free(curr);
}

seq_t a(idx_t n) {
    if (n == 1) return 2;
    
    seq_t P_n, P_n_minus_1;
    compute_fubini_pair(n, &P_n, &P_n_minus_1);
    
    seq_t power_of_2 = pow2(n + 1);
    seq_t result = power_of_2 * (P_n - (seq_t)n * P_n_minus_1);
    
    return result;
}
