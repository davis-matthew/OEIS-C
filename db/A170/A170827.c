/*
 * A170827  Sum of digits after the decimal point in the decimal expansion of
 *          (3/2)^n.
 * https://oeis.org/A170827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of decimal digits of 15^n mod 10^n
 * Range:     u8 n <= 55, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the decimal expansion of (3/2)^n
   by accumulating 15^n as a base‑10 integer and then taking the
   lowest n digits.  The sum of those digits is the required
   sequence value.  It works for any size of seq_t (unsigned
   integer or arbitrary‑precision class) because the intermediate
   arithmetic is performed on a byte array and the final result
   is a small integer that always fits in seq_t. */

#include <stdlib.h>
#include <stdint.h>

/* The function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Allocate enough space for the full decimal representation of 15^n.
       15^n has at most ceil(n * log10(15)) ≈ 1.176*n digits. */
    size_t max_digits = (size_t)(n * 2) + 10;          /* safety margin */
    uint8_t *dig = (uint8_t *)malloc(max_digits);
    if (!dig) return 0;                               /* allocation failure */

    dig[0] = 1;                                        /* 15^0 */
    size_t len = 1;                                    

    for (idx_t step = 1; step <= n; ++step) {
        uint32_t carry = 0;                           /* product fits in 32 bits */
        for (size_t i = 0; i < len; ++i) {
            uint32_t prod = (uint32_t)dig[i] * 15 + carry;
            dig[i] = (uint8_t)(prod % 10);
            carry  = prod / 10;
        }
        /* Handle remaining carry digits. */
        while (carry) {
            dig[len++] = (uint8_t)(carry % 10);
            carry     /= 10;
        }
    }

    /* Sum the lowest n decimal digits (i.e., the fractional part). */
    uint64_t sum = 0;                                    /* fits in seq_t for all n */
    for (size_t i = 0; i < n && i < len; ++i) sum += dig[i];

    free(dig);
    return (seq_t)sum;
}
