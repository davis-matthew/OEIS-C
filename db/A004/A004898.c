/*
 * A004898  Numbers that are the sum of at most 3 nonzero 10th powers.
 * https://oeis.org/A004898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers that are the sum of at most 3 nonzero 10th powers
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


static seq_t *sequence = NULL;
static idx_t sequence_len = 0;

static int compare_seq_t(const void *a, const void *b) {
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

seq_t a(idx_t n) {
    if (sequence == NULL) {
        seq_t *powers = (seq_t *)malloc(300 * sizeof(seq_t));
        idx_t num_powers = 0;
        seq_t prev_power = 0;
        
        for (idx_t base = 1; base <= 2000; base++) {
            seq_t power = 1;
            for (int i = 0; i < 10; i++) {
                power = power * base;
            }
            
            if (base > 1 && power <= prev_power) {
                break;
            }
            
            if (num_powers >= 300) break;
            
            powers[num_powers++] = power;
            prev_power = power;
        }
        
        idx_t m = num_powers;
        idx_t capacity = 1 + m + m * (m + 1) / 2 + m * (m + 1) * (m + 2) / 6 + 100;
        
        seq_t *temp = (seq_t *)malloc(capacity * sizeof(seq_t));
        idx_t count = 0;
        
        temp[count++] = 0;
        
        for (idx_t i = 0; i < num_powers; i++) {
            temp[count++] = powers[i];
        }
        
        for (idx_t i = 0; i < num_powers; i++) {
            for (idx_t j = i; j < num_powers; j++) {
                if (count < capacity) {
                    temp[count++] = powers[i] + powers[j];
                }
            }
        }
        
        for (idx_t i = 0; i < num_powers; i++) {
            for (idx_t j = i; j < num_powers; j++) {
                for (idx_t k = j; k < num_powers; k++) {
                    if (count < capacity) {
                        temp[count++] = powers[i] + powers[j] + powers[k];
                    }
                }
            }
        }
        
        free(powers);
        
        qsort(temp, count, sizeof(seq_t), compare_seq_t);
        
        idx_t unique_count = 0;
        for (idx_t i = 0; i < count; i++) {
            if (i == 0 || temp[i] != temp[i - 1]) {
                temp[unique_count++] = temp[i];
            }
        }
        
        sequence = (seq_t *)malloc(unique_count * sizeof(seq_t));
        memcpy(sequence, temp, unique_count * sizeof(seq_t));
        sequence_len = unique_count;
        
        free(temp);
    }
    
    return sequence[n - 1];
}
