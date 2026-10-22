/*
 * A000347  Number of partitions into non-integral powers.
 * https://oeis.org/A000347
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/* Brute-force search: count ordered 4-tuples (x1,x2,x3,x4) with 1<=x1<=x2<=x3<=x4 and sqrt(x1)+sqrt(x2)+sqrt(x3)+sqrt(x4)<=n */

seq_t a(idx_t n) {
    seq_t count = 0;
    double n_f = (double)n;
    
    for (idx_t x1 = 1; ; x1++) {
        double sqrt_x1 = sqrt((double)x1);
        if (sqrt_x1 > n_f) break;
        
        for (idx_t x2 = x1; ; x2++) {
            double sum_12 = sqrt_x1 + sqrt((double)x2);
            if (sum_12 > n_f) break;
            
            for (idx_t x3 = x2; ; x3++) {
                double sum_123 = sum_12 + sqrt((double)x3);
                if (sum_123 > n_f) break;
                
                double remaining = n_f - sum_123;
                idx_t max_x4 = (idx_t)(remaining * remaining);
                if (max_x4 >= x3) {
                    count += (seq_t)(max_x4 - x3 + 1);
                }
            }
        }
    }
    
    return count;
}
