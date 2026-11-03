/*
 * A007730  7th binary partition function.
 * https://oeis.org/A007730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Product_{k>=0} (1 - x^(7*2^k))/(1 - x^(2^k))
 * Range:     u8 n <= 67, u16 n <= 1503, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9298 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"


seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    
    seq_t *coeff = mem_new(seq_t, (size_t)n + 1);
    seq_t *temp = mem_new(seq_t, (size_t)n + 1);
    if (!coeff || !temp) {
        mem_free(coeff);
        mem_free(temp);
        return 0;
    }
    coeff[0] = (seq_t)1;
    
    // Apply factors for k = 0 to log2(n)
    for (int k = 0; k < 64; ++k) {
        idx_t d = ((idx_t)1) << k;
        if (d > n) break;
        
        // Clear temp array
        mem_set(temp, 0, ((size_t)n + 1) * sizeof(seq_t));
        
        // Multiply by factor (1 + x^d + x^(2d) + ... + x^(6d))
        for (idx_t i = 0; i <= n; ++i) {
            for (int j = 0; j <= 6; ++j) {
                if ((idx_t)j * d > i) break;
                idx_t idx = i - (idx_t)j * d;
                temp[i] += coeff[idx];
            }
        }
        
        // Swap coeff and temp
        seq_t *tmp = coeff;
        coeff = temp;
        temp = tmp;
    }
    
    seq_t result = coeff[n];
    mem_free(coeff);
    mem_free(temp);
    return result;
}
