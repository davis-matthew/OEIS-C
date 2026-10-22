/*
 * A004152  Sum of digits of n!.
 * https://oeis.org/A004152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of digits of n!
 * Range:     u8 n <= 51, u16 n <= 4807, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return 1;
    
    uint64_t max_digits = (uint64_t)(n * 3.7 + 100);
    uint8_t *digits = (uint8_t *)malloc(max_digits);
    digits[0] = 1;
    uint64_t num_digits = 1;
    
    for (idx_t i = 2; i <= n; i++) {
        uint64_t carry = 0;
        for (uint64_t j = 0; j < num_digits; j++) {
            uint64_t product = (uint64_t)digits[j] * (uint64_t)i + carry;
            digits[j] = product % 10;
            carry = product / 10;
        }
        
        while (carry > 0) {
            digits[num_digits] = carry % 10;
            carry /= 10;
            num_digits++;
        }
    }
    
    seq_t result = 0;
    for (uint64_t i = 0; i < num_digits; i++) {
        result += digits[i];
    }
    
    free(digits);
    return result;
}
