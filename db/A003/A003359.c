/*
 * A003359  Numbers that are the sum of 3 nonzero 6th powers.
 * https://oeis.org/A003359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Generate all sums of 3 nonzero 6th powers a^6 + b^6 + c^6 where
 *            1 <= a <= b <= c, sort, and return the n-th term
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   cmp_seq_t_values
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/cmp_seq_t_values.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    static seq_t *sums = NULL;
    static idx_t unique_count = 0;
    
    if (sums == NULL) {
        int limit = 180;
        
        // Precompute 6th powers
        seq_t *pow6 = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
        for (int i = 1; i <= limit; i++) {
            seq_t x = (seq_t)i;
            seq_t x3 = x * x * x;
            pow6[i] = x3 * x3;
        }
        
        // Allocate space for all sums
        idx_t capacity = (limit + 1) * (limit + 2) * (limit + 3) / 6 + 1000;
        sums = (seq_t *)malloc(capacity * sizeof(seq_t));
        idx_t count = 0;
        
        // Generate all sums a^6 + b^6 + c^6 with 1 <= a <= b <= c <= limit
        for (int a = 1; a <= limit; a++) {
            for (int b = a; b <= limit; b++) {
                for (int c = b; c <= limit; c++) {
                    sums[count++] = pow6[a] + pow6[b] + pow6[c];
                }
            }
        }
        
        // Sort all sums
        qsort(sums, count, sizeof(seq_t), cmp_seq_t_values);
        
        // Remove duplicates while maintaining sorted order
        unique_count = 0;
        if (count > 0) {
            sums[unique_count++] = sums[0];
            for (idx_t i = 1; i < count; i++) {
                if (sums[i] != sums[i-1]) {
                    sums[unique_count++] = sums[i];
                }
            }
        }
        
        free(pow6);
    }
    
    // Return n-th term (1-indexed)
    if (n <= unique_count) {
        return sums[n - 1];
    }
    
    return 0;
}
