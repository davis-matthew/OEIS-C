/*
 * A003336  Numbers that are the sum of 2 positive 4th powers.
 * https://oeis.org/A003336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th number that is the sum of two positive 4th powers
 *            x^4 + y^4 with x <= y
 * Range:     u8 none, u16 none, u32 n <= 9828, u64 n <= 9828, u128 n <= 9828,
 *            big any n
 * Helpers:   cmp_func
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/cmp_func.h"

static seq_t *computed = NULL;
static idx_t max_computed = 0;

seq_t a(idx_t n) {
    if (computed != NULL && n <= max_computed) {
        return computed[n - 1];
    }
    
    idx_t y_max = 20;
    if (n > 1000) y_max = 100;
    if (n > 10000) y_max = 300;
    
    seq_t *sums = NULL;
    idx_t count = 0;
    idx_t unique_count = 0;
    
    while (unique_count < n && y_max < 100000) {
        free(sums);
        
        idx_t cap = y_max * (y_max + 1) / 2 + 100;
        sums = (seq_t *)malloc(cap * sizeof(seq_t));
        count = 0;
        
        for (idx_t x = 1; x <= y_max; x++) {
            seq_t xs = (seq_t)x;
            seq_t x4 = xs * xs * xs * xs;
            for (idx_t y = x; y <= y_max; y++) {
                seq_t ys = (seq_t)y;
                seq_t y4 = ys * ys * ys * ys;
                sums[count++] = x4 + y4;
            }
        }
        
        qsort(sums, count, sizeof(seq_t), cmp_func);
        
        unique_count = 0;
        for (idx_t i = 0; i < count; i++) {
            if (i == 0 || sums[i] != sums[i - 1]) {
                unique_count++;
            }
        }
        
        if (unique_count < n) {
            y_max = (y_max * 3) / 2 + 1;
        }
    }
    
    if (computed == NULL) {
        computed = (seq_t *)malloc(unique_count * sizeof(seq_t));
    } else if (unique_count > max_computed) {
        computed = (seq_t *)realloc(computed, unique_count * sizeof(seq_t));
    }
    
    max_computed = 0;
    for (idx_t i = 0; i < count; i++) {
        if (i == 0 || sums[i] != sums[i - 1]) {
            computed[max_computed++] = sums[i];
        }
    }
    
    free(sums);
    
    return computed[n - 1];
}
