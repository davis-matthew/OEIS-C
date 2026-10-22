/*
 * A004071  Start with a(0)=1; replace each i with 12...i, then add 1 to final
 *          digit.
 * https://oeis.org/A004071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Start with 1; repeatedly replace each digit i with "123...i",
 *            then increment the final digit by 1. a(n) is the result after n
 *            iterations
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    // Build the digit string iteratively
    char *str = (char*)malloc(2);
    str[0] = '1';
    str[1] = '\0';
    idx_t len = 1;
    
    for (idx_t step = 0; step < n; step++) {
        // Allocate new string for transformed digits
        char *new_str = (char*)malloc(len * 9 + 1);
        idx_t new_len = 0;
        
        // Replace each digit d with "123...d"
        for (idx_t i = 0; i < len; i++) {
            int digit_val = str[i] - '0';
            for (int j = 1; j <= digit_val; j++) {
                new_str[new_len++] = '0' + j;
            }
        }
        
        // Increment last digit
        if (new_len > 0) {
            new_str[new_len - 1]++;
        }
        
        free(str);
        str = new_str;
        len = new_len;
    }
    
    // Convert string to seq_t
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i < len; i++) {
        result = result * (seq_t)10 + (seq_t)(str[i] - '0');
    }
    
    free(str);
    return result;
}
