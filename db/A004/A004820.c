/*
 * A004820  Numbers that are the sum of 9 positive 11th powers.
 * https://oeis.org/A004820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A004820 - numbers that are sums of 9 positive 11th powers
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   cmp_seq_func
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/cmp_seq_func.h"

static seq_t *cache = NULL;
static idx_t cache_count = 0;

static void init_cache(void) {
    idx_t max_base = 15;
    
    seq_t powers[16];
    for (idx_t i = 1; i <= max_base; i++) {
        seq_t result = (seq_t)i;
        for (int j = 0; j < 10; j++) {
            result = result * (seq_t)i;
        }
        powers[i] = result;
    }
    
    seq_t *temp = (seq_t *)malloc(2000000 * sizeof(seq_t));
    idx_t temp_count = 0;
    
    for (idx_t a = 1; a <= max_base; a++) {
        seq_t pow_a = powers[a];
        for (idx_t b = a; b <= max_base; b++) {
            seq_t pow_b = powers[b];
            for (idx_t c = b; c <= max_base; c++) {
                seq_t pow_c = powers[c];
                for (idx_t d = c; d <= max_base; d++) {
                    seq_t pow_d = powers[d];
                    for (idx_t e = d; e <= max_base; e++) {
                        seq_t pow_e = powers[e];
                        for (idx_t f = e; f <= max_base; f++) {
                            seq_t pow_f = powers[f];
                            for (idx_t g = f; g <= max_base; g++) {
                                seq_t pow_g = powers[g];
                                for (idx_t h = g; h <= max_base; h++) {
                                    seq_t pow_h = powers[h];
                                    for (idx_t i = h; i <= max_base; i++) {
                                        seq_t pow_i = powers[i];
                                        seq_t sum = pow_a + pow_b + pow_c + pow_d + pow_e + pow_f + pow_g + pow_h + pow_i;
                                        temp[temp_count++] = sum;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    qsort(temp, temp_count, sizeof(seq_t), cmp_seq_func);
    
    cache = (seq_t *)malloc(temp_count * sizeof(seq_t));
    
    cache_count = 0;
    for (idx_t i = 0; i < temp_count; i++) {
        if (cache_count == 0 || cache[cache_count - 1] != temp[i]) {
            cache[cache_count++] = temp[i];
        }
    }
    
    free(temp);
}

seq_t a(idx_t n) {
    if (cache == NULL) {
        init_cache();
    }
    
    return cache[n - 1];
}
