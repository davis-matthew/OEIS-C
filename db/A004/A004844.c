/*
 * A004844  Numbers that are the sum of at most 4 positive 5th powers.
 * https://oeis.org/A004844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers that are the sum of at most 4 positive 5th powers, in
 *            increasing order
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static int compare_seq_t(const void *a, const void *b) {
    seq_t va = *(seq_t *)a;
    seq_t vb = *(seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

seq_t a(idx_t n) {
    static seq_t *sorted_values = NULL;
    static idx_t num_values = 0;
    
    if (sorted_values == NULL) {
        size_t buffer_size = 5000000;
        seq_t *temp_sums = (seq_t *)malloc(buffer_size * sizeof(seq_t));
        size_t count = 0;
        
        /* Generate fifth powers for bases 1-40 */
        seq_t fifth[40];
        for (int base = 1; base <= 40; base++) {
            fifth[base - 1] = (seq_t)base * (seq_t)base * (seq_t)base * (seq_t)base * (seq_t)base;
        }
        
        /* Generate all sums of 0, 1, 2, 3, 4 fifth powers */
        temp_sums[count++] = 0;
        
        for (int i = 0; i < 40 && count < buffer_size; i++) {
            temp_sums[count++] = fifth[i];
        }
        
        for (int i = 0; i < 40 && count < buffer_size; i++) {
            for (int j = 0; j < 40 && count < buffer_size; j++) {
                temp_sums[count++] = fifth[i] + fifth[j];
            }
        }
        
        for (int i = 0; i < 40 && count < buffer_size; i++) {
            for (int j = 0; j < 40 && count < buffer_size; j++) {
                for (int k = 0; k < 40 && count < buffer_size; k++) {
                    temp_sums[count++] = fifth[i] + fifth[j] + fifth[k];
                }
            }
        }
        
        for (int i = 0; i < 40 && count < buffer_size; i++) {
            for (int j = 0; j < 40 && count < buffer_size; j++) {
                for (int k = 0; k < 40 && count < buffer_size; k++) {
                    for (int l = 0; l < 40 && count < buffer_size; l++) {
                        temp_sums[count++] = fifth[i] + fifth[j] + fifth[k] + fifth[l];
                    }
                }
            }
        }
        
        qsort(temp_sums, count, sizeof(seq_t), compare_seq_t);
        
        sorted_values = (seq_t *)malloc((count + 1) * sizeof(seq_t));
        num_values = 0;
        for (size_t i = 0; i < count; i++) {
            if (i == 0 || temp_sums[i] != temp_sums[i - 1]) {
                sorted_values[num_values++] = temp_sums[i];
            }
        }
        
        free(temp_sums);
    }
    
    return sorted_values[n - 1];
}
