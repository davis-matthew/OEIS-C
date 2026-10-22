/*
 * A001388  Describe the previous term (in base 3)!.
 * https://oeis.org/A001388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Look-and-say sequence in base 3. a(n) describes a(n-1) by
 *            counting consecutive runs of digits (in base 3), writing each
 *            run as: (count in base 3) (digit)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>


seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    size_t capacity = 10000000;
    uint8_t *digits = (uint8_t *)malloc(capacity);
    if (!digits) return 0;
    
    int len = 1;
    digits[0] = 1;
    
    for (idx_t term_idx = 2; term_idx <= n; term_idx++) {
        uint8_t *next_digits = (uint8_t *)malloc(capacity);
        if (!next_digits) {
            free(digits);
            return 0;
        }
        
        int next_len = 0;
        for (int i = 0; i < len; ) {
            uint8_t digit = digits[i];
            int count = 1;
            while (i + count < len && digits[i + count] == digit) {
                count++;
            }
            
            // Write count in base 3
            if (count < 3) {
                next_digits[next_len++] = (uint8_t)count;
            } else {
                uint8_t base3[30];
                int b3_len = 0;
                int c = count;
                while (c > 0) {
                    base3[b3_len++] = c % 3;
                    c /= 3;
                }
                for (int j = b3_len - 1; j >= 0; j--) {
                    next_digits[next_len++] = base3[j];
                }
            }
            
            next_digits[next_len++] = digit;
            i += count;
        }
        
        if (next_len > (int)capacity) {
            free(next_digits);
            free(digits);
            return 0;
        }
        
        memcpy(digits, next_digits, next_len);
        len = next_len;
        free(next_digits);
    }
    
    seq_t result = 0;
    for (int i = 0; i < len; i++) {
        result = result * 10 + digits[i];
    }
    
    free(digits);
    return result;
}
