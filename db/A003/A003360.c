/*
 * A003360  Numbers that are the sum of 4 positive 6th powers.
 * https://oeis.org/A003360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sum of 4 positive 6th powers, sorted in ascending order
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static int cache_initialized = 0;

int compare_seq_t(const void *a, const void *b) {
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

seq_t a(idx_t n) {
    if (!cache_initialized) {
        cache_initialized = 1;
        
        idx_t max_base = 50;
        idx_t capacity = 300000;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        
        cache_len = 0;
        for (idx_t a_val = 1; a_val <= max_base; a_val++) {
            seq_t a6 = (seq_t)a_val;
            a6 = a6 * a6 * a6;
            a6 = a6 * a6;
            
            for (idx_t b_val = a_val; b_val <= max_base; b_val++) {
                seq_t b6 = (seq_t)b_val;
                b6 = b6 * b6 * b6;
                b6 = b6 * b6;
                
                for (idx_t c_val = b_val; c_val <= max_base; c_val++) {
                    seq_t c6 = (seq_t)c_val;
                    c6 = c6 * c6 * c6;
                    c6 = c6 * c6;
                    
                    for (idx_t d_val = c_val; d_val <= max_base; d_val++) {
                        seq_t d6 = (seq_t)d_val;
                        d6 = d6 * d6 * d6;
                        d6 = d6 * d6;
                        
                        if (cache_len < capacity) {
                            cache[cache_len] = a6 + b6 + c6 + d6;
                            cache_len++;
                        }
                    }
                }
            }
        }
        
        qsort(cache, cache_len, sizeof(seq_t), compare_seq_t);
        
        idx_t write_pos = 0;
        for (idx_t i = 0; i < cache_len; i++) {
            if (i == 0 || cache[i] != cache[i-1]) {
                cache[write_pos] = cache[i];
                write_pos++;
            }
        }
        cache_len = write_pos;
    }
    
    if (n >= 1 && n <= cache_len) {
        return cache[n - 1];
    }
    
    return 0;
}
