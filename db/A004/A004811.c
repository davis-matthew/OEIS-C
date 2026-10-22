/*
 * A004811  Numbers that are the sum of 11 positive 10th powers.
 * https://oeis.org/A004811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sum of exactly 11 positive 10th powers, ordered by value
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   cmp_seq_vals
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/cmp_seq_vals.h"

static seq_t pow10(uint64_t x) {
    seq_t x_seq = (seq_t)x;
    seq_t x2 = x_seq * x_seq;
    seq_t x4 = x2 * x2;
    seq_t x8 = x4 * x4;
    return x8 * x2;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (n > 0 && n <= cache_len) {
        return cache[n - 1];
    }
    
    uint64_t max_base = 12;
    
    while (cache_len < n) {
        idx_t max_alloc = 100000000;
        seq_t *sums = (seq_t *)malloc(sizeof(seq_t) * max_alloc);
        idx_t sums_len = 0;
        
        uint64_t indices[11];
        for (int i = 0; i < 11; i++) {
            indices[i] = 1;
        }
        
        while (sums_len < max_alloc) {
            seq_t sum = 0;
            for (int i = 0; i < 11; i++) {
                sum += pow10(indices[i]);
            }
            sums[sums_len++] = sum;
            
            int i = 10;
            while (i >= 0 && indices[i] == max_base) {
                i--;
            }
            if (i < 0) break;
            
            indices[i]++;
            for (int j = i + 1; j < 11; j++) {
                indices[j] = indices[i];
            }
        }
        
        qsort(sums, sums_len, sizeof(seq_t), cmp_seq_vals);
        
        idx_t unique_len = 0;
        for (idx_t i = 0; i < sums_len; i++) {
            if (i == 0 || sums[i] != sums[i-1]) {
                sums[unique_len++] = sums[i];
            }
        }
        
        if (cache) free(cache);
        cache = sums;
        cache_len = unique_len;
        
        if (cache_len >= n) break;
        max_base += 2;
    }
    
    if (n > 0 && n <= cache_len) {
        return cache[n - 1];
    }
    
    return 0;
}
