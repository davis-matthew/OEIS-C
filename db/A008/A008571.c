/*
 * A008571  Digits of powers of 11.
 * https://oeis.org/A008571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, tabf
 * Formula:   a(n) = nth decimal digit of the concatenated powers of 11
 *            (powers 11^0, 11^1, 11^2, …). The value fits in 0..9
 * Range:     u8 n <= 5310, u16 n <= 5310, u32 n <= 5310, u64 n <= 5310,
 *            u128 n <= 5310, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Current decimal representation of 11^k.
       Least‑significant digit stored at index 0.  */
    size_t len      = 1;
    uint8_t *digits = (uint8_t *)malloc(len);
    if (!digits) return 0;          /* allocation failure – unlikely */
    digits[0] = 1;                  /* 11^0 = 1 */

    idx_t pos = 0;                  /* position of next digit to read */

    while (1) {
        /* Emit digits from most significant to least significant. */
        for (size_t i = len; i-- > 0;) {
            if (pos == n) {
                seq_t result = (seq_t)digits[i];
                free(digits);
                return result;
            }
            ++pos;
        }

        /* Multiply the current number by 11 to obtain the next power. */
        size_t carry = 0;
        for (size_t i = 0; i < len; ++i) {
            size_t prod = (size_t)digits[i] * 11 + carry;
            digits[i]    = (uint8_t)(prod % 10);
            carry        = prod / 10;
        }
        while (carry) {
            uint8_t new_digit = (uint8_t)(carry % 10);
            carry /= 10;
            digits = (uint8_t *)realloc(digits, len + 1);
            if (!digits) return 0;   /* allocation failure */
            digits[len++] = new_digit;
        }
    }
}
