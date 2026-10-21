/*
 * A065713  Sum of digits of 4^n.
 * https://oeis.org/A065713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of digits of 4^n
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

extern seq_t digit_sum(seq_t v);  /* included for compatibility with utils */

seq_t a(idx_t n)
{
    /* We compute 4^n in base 10^9 and sum its decimal digits. */
    /* Allocate a vector that is certainly large enough. */
    size_t max_words = (size_t)n + 2;           /* safety margin */
    uint32_t *digits = (uint32_t*)malloc(max_words * sizeof(uint32_t));
    if (!digits) return (seq_t)0;              /* out of memory – return 0 */

    size_t len = 1;
    digits[0] = 1;                              /* 4^0 = 1 */

    for (idx_t i = 0; i < n; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint64_t prod = (uint64_t)digits[j] * 4 + carry;
            digits[j] = (uint32_t)(prod % 1000000000ULL);
            carry    = prod / 1000000000ULL;
        }
        if (carry) {
            digits[len++] = (uint32_t)carry;
        }
    }

    /* Sum decimal digits of the big integer. */
    seq_t sum = (seq_t)0;
    for (size_t j = 0; j < len; ++j) {
        uint32_t t = digits[j];
        for (int k = 0; k < 9; ++k) {          /* each word has ≤9 decimal digits */
            sum += (seq_t)(t % 10);
            t   /= 10;
        }
    }

    free(digits);
    return sum;
}
