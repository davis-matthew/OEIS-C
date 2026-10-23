/*
 * A116943  Number of 4s digits plus non-final 3s digits 3 base 5 expansion of
 *          2^n.
 * https://oeis.org/A116943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of 4‑digits plus non‑final 3‑digits in the base‑5
 *            expansion of 2^n
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* allocate a buffer big enough for all digits of 2^n in base 5.
       The number of base‑5 digits of 2^n is about n*log5(2)+1 < 0.431*n+1.
       For n ≤ 10000 this is < 4300, so an array of n+10 is plenty. */
    size_t cap = (size_t)n + 10;
    uint8_t *digits = (uint8_t *)malloc(cap);
    if (!digits) return 0;  /* out of memory; return 0 as a safe fallback */

    size_t len = 1;
    digits[0] = 1;          /* 2^0 == 1 in base 5 */

    /* multiply the current big integer (in base 5) by 2, n times */
    for (idx_t i = 0; i < n; ++i) {
        uint8_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            /* use 16‑bit to hold the intermediate product */
            uint16_t prod = (uint16_t)digits[j] * 2 + carry;
            digits[j] = (uint8_t)(prod % 5);
            carry   = (uint8_t)(prod / 5);
        }
        while (carry) {
            digits[len++] = carry % 5;
            carry /= 5;
        }
    }

    /* count the required digits:
       - least‑significant digit (index 0) counts only if it is 4
       - all other digits count if they are 3 or 4 */
    seq_t count = 0;
    if (digits[0] == 4) count++;
    for (size_t j = 1; j < len; ++j) {
        if (digits[j] == 3 || digits[j] == 4) count++;
    }

    free(digits);
    return count;
}
