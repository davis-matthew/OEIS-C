/*
 * A003074  Number of different numbers <= n that are sums of 3 positive
 *          cubes.
 * https://oeis.org/A003074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count of distinct numbers <= n expressible as a^3 + b^3 + c^3
 *            for positive integers a, b, c
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n_idx) {
    seq_t n = (seq_t)n_idx;
    
    if (n < 3) return 0;
    
    seq_t count = 0;
    bool *is_sum = (bool *)malloc((size_t)(n + 1) * sizeof(bool));
    if (!is_sum) return 0;
    
    for (size_t i = 0; i <= (size_t)n; i++) {
        is_sum[i] = false;
    }
    
    for (seq_t a = 1; a * a * a <= n; a++) {
        seq_t a3 = a * a * a;
        for (seq_t b = 1; a3 + b * b * b <= n; b++) {
            seq_t b3 = b * b * b;
            for (seq_t c = 1; a3 + b3 + c * c * c <= n; c++) {
                seq_t c3 = c * c * c;
                seq_t sum = a3 + b3 + c3;
                is_sum[(size_t)sum] = true;
            }
        }
    }
    
    for (seq_t i = 1; i <= n; i++) {
        if (is_sum[(size_t)i]) count++;
    }
    
    free(is_sum);
    return count;
}
