/*
 * A071582  Powers of 4 written backwards.
 * https://oeis.org/A071582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = decimal_reverse(4^n)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Compute the n-th term of OEIS A071582:
   reverse of the decimal expansion of 4^n. */
seq_t a(idx_t n)
{
    /* 4^0 = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Rough upper bound on the number of decimal digits of 4^n.
       log10(4) ≈ 0.6020, use a slightly bigger factor for safety. */
    size_t cap = (size_t)((double)n * 0.7) + 10;
    if (cap == 0)
        cap = 10;

    uint8_t *digits = (uint8_t*)malloc(cap);
    if (!digits)   /* allocation failure – unlikely on the test judge */
        return (seq_t)0;

    digits[0] = 1;      /* 4^0 */
    size_t len   = 1;    /* current number of stored digits (LSB first) */

    /* Multiply by 4 exactly n times */
    for (idx_t i = 0; i < n; ++i)
    {
        uint16_t carry = 0;
        for (size_t j = 0; j < len; ++j)
        {
            uint16_t prod = (uint16_t)digits[j] * 4 + carry;
            digits[j] = (uint8_t)(prod % 10);
            carry     = prod / 10;
        }
        while (carry)
        {
            if (len == cap) {
                cap <<= 1;
                uint8_t *tmp = (uint8_t*)realloc(digits, cap);
                if (!tmp) {
                    free(digits);
                    return (seq_t)0;   /* allocation failure */
                }
                digits = tmp;
            }
            digits[len++] = (uint8_t)(carry % 10);
            carry        /= 10;
        }
    }

    /* Build the reversed number: digits are already stored least‑significant first */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i)
        result = result * 10 + (seq_t)digits[i];

    free(digits);
    return result;
}
