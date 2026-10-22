/*
 * A004801  Sum of 12 positive 9th powers.
 * https://oeis.org/A004801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest sum of 12 positive 9th powers
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   cmp_seq_val, generate_sums_helper, ninth_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/cmp_seq_val.h"
#include "../utils/generate_sums_helper.h"
#include "../utils/ninth_power.h"

seq_t a(idx_t n) {
    static seq_t *sums = NULL;
    static idx_t count = 0;
    static bool computed = false;
    
    if (!computed) {
        idx_t capacity = 20000;
        sums = (seq_t *)malloc(capacity * sizeof(seq_t));
        
        idx_t tuple[12];
        idx_t max_base = 7;
        
        generate_sums_helper(&sums, &count, &capacity, 0, 1, max_base, tuple);
        
        qsort(sums, (size_t)count, sizeof(seq_t), cmp_seq_val);
        
        idx_t write_pos = 0;
        for (idx_t i = 0; i < count; i++) {
            if (i == 0 || sums[i] != sums[i-1]) {
                sums[write_pos++] = sums[i];
            }
        }
        count = write_pos;
        
        computed = true;
    }
    
    if (n > count) {
        return 0;
    }
    
    return sums[n - 1];
}
