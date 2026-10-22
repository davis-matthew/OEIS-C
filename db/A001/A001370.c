/*
 * A001370  Sum of digits of 2^n.
 * https://oeis.org/A001370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = A007953(A000079(n))
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    // Allocate space for digits of 2^n
    // Number of digits is approximately n * log10(2) + 1
    idx_t max_digits = ((n * 302) / 1000) + 10;
    
    uint8_t *digits = (uint8_t *)malloc(max_digits * sizeof(uint8_t));
    digits[0] = 1;
    idx_t len = 1;
    
    // Compute 2^n by multiplying by 2, n times
    for (idx_t i = 0; i < n; i++) {
        uint32_t carry = 0;
        for (idx_t j = 0; j < len; j++) {
            uint32_t prod = (uint32_t)digits[j] * 2 + carry;
            digits[j] = prod % 10;
            carry = prod / 10;
        }
        if (carry > 0) {
            digits[len++] = (uint8_t)carry;
        }
    }
    
    // Sum all digits
    seq_t sum = 0;
    for (idx_t i = 0; i < len; i++) {
        sum += (seq_t)digits[i];
    }
    
    free(digits);
    return sum;
}
