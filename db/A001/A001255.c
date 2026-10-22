/*
 * A001255  Squares of partition numbers.
 * https://oeis.org/A001255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000041(n)^2
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 43, u64 n <= 127, u128 n <= 416,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *partitions = NULL;
static idx_t max_computed = 0;

static void compute_partition_up_to(idx_t n) {
    if (partitions == NULL) {
        partitions = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (partitions == NULL) return;
        partitions[0] = 1;
        max_computed = 0;
    } else if (n <= max_computed) {
        return;
    } else {
        seq_t *temp = (seq_t *)realloc(partitions, (n + 1) * sizeof(seq_t));
        if (temp == NULL) return;
        partitions = temp;
    }
    
    for (idx_t i = max_computed + 1; i <= n; i++) {
        seq_t sum = 0;
        for (idx_t k = 1; ; k++) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            idx_t pent2 = k * (3 * k + 1) / 2;
            
            if (pent1 > i) break;
            
            int sign = ((k - 1) % 2 == 0) ? 1 : -1;
            
            if (sign > 0) {
                sum += partitions[i - pent1];
            } else {
                sum -= partitions[i - pent1];
            }
            
            if (pent2 <= i) {
                if (sign > 0) {
                    sum += partitions[i - pent2];
                } else {
                    sum -= partitions[i - pent2];
                }
            }
        }
        partitions[i] = sum;
    }
    
    max_computed = n;
}

seq_t a(idx_t n) {
    compute_partition_up_to(n);
    seq_t p = partitions[n];
    return p * p;
}
