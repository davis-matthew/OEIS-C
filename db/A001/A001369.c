/*
 * A001369  Blocks of increasing length using 1,2,3,...,9,10; omit leading
 *          0's.
 * https://oeis.org/A001369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Blocks of increasing length from repeating sequence 1,2,...,9,10
 *            * written as "12345678910". Block n is the n-th partition, with
 *            leading 0s omitted
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
        
    // Block n occupies positions [n(n-1)/2, n(n-1)/2 + n)
    idx_t start_pos = n * (n - 1) / 2;
    idx_t block_len = n;
    
    char *block = (char *)malloc(block_len + 1);
    
    // Extract digits from repeating cycle "12345678910"
    for (idx_t i = 0; i < block_len; i++) {
        idx_t pos = start_pos + i;
        idx_t offset = pos % 11;
        
        if (offset < 9) {
            block[i] = '1' + offset;  // Digits '1' through '9'
        } else if (offset == 9) {
            block[i] = '1';  // First digit of "10"
        } else {  // offset == 10
            block[i] = '0';  // Second digit of "10"
        }
    }
    block[block_len] = '\0';
    
    // Skip leading zeros
    idx_t start_idx = 0;
    while (start_idx < block_len && block[start_idx] == '0') {
        start_idx++;
    }
    
    // Convert to seq_t
    seq_t result = 0;
    for (idx_t i = start_idx; i < block_len; i++) {
        result = result * 10 + (block[i] - '0');
    }
    
    free(block);
    return result;
}
