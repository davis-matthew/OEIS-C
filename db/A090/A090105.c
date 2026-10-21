/*
 * A090105  Decimal value of largest digit of n! when it was written in base
 *          n.
 * https://oeis.org/A090105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = largest digit of n! when written in base n (decimal
 *            value)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (n == 1) return 1;

    uint32_t *digits = (uint32_t *)malloc(sizeof(uint32_t));
    size_t capacity = 1, len = 1;
    digits[0] = 1;

    for (idx_t k = 2; k <= n; ++k) {
        uint64_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            uint64_t prod = (uint64_t)digits[i] * k + carry;
            digits[i] = (uint32_t)(prod % n);
            carry    = prod / n;
        }
        while (carry) {
            if (len == capacity) {
                capacity <<= 1;
                digits = (uint32_t *)realloc(digits, capacity * sizeof(uint32_t));
            }
            digits[len++] = (uint32_t)(carry % n);
            carry /= n;
        }
    }

    uint32_t max_digit = 0;
    for (size_t i = 0; i < len; ++i)
        if (digits[i] > max_digit) max_digit = digits[i];

    free(digits);
    return (seq_t)max_digit;
}
