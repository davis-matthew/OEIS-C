/*
 * A003549  Divisors of 2^44 - 1.
 * https://oeis.org/A003549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   divisors of 2^44 - 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 65, u64 n <= 128, u128 n <= 128,
 *            big any n
 * Helpers:   compare_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/compare_divisor.h"

seq_t a(idx_t n) {
    static seq_t *divisors = NULL;
    static idx_t divisor_count = 0;
    
    if (divisors == NULL) {
        uint64_t num = 17592186044415ULL;
        seq_t *temp = (seq_t *)malloc(256 * sizeof(seq_t));
        divisor_count = 0;
        
        for (uint64_t i = 1; i * i <= num; i++) {
            if (num % i == 0) {
                temp[divisor_count++] = (seq_t)i;
                if (i * i != num) {
                    temp[divisor_count++] = (seq_t)(num / i);
                }
            }
        }
        
        qsort(temp, divisor_count, sizeof(seq_t), compare_divisor);
        divisors = temp;
    }
    
    return divisors[n - 1];
}
