/*
 * A127110  n! in base 3.
 * https://oeis.org/A127110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal integer formed by writing n! in base 3 and
 *            interpreting that digit string as a decimal number
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Compute n! as a seq_t value. */
    seq_t fac = factorial(n);

    /* Extract base‑3 digits of fac (least significant first). */
    size_t cap = 64;
    size_t len = 0;
    uint8_t *digits = (uint8_t *)malloc(cap * sizeof(uint8_t));
    if (!digits)
        return (seq_t)0; /* allocation failure */

    while (fac != (seq_t)0) {
        seq_t d = fac % (seq_t)3;   /* digit: 0,1,2 */
        fac = fac / (seq_t)3;
        if (len >= cap) {
            cap <<= 1;              /* double capacity */
            uint8_t *tmp = (uint8_t *)realloc(digits, cap * sizeof(uint8_t));
            if (!tmp) {
                free(digits);
                return (seq_t)0;
            }
            digits = tmp;
        }
        digits[len++] = (uint8_t)d;
    }

    /* Build the decimal number from most significant digit to least. */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < len; ++i) {
        seq_t digit = (seq_t)digits[len - 1 - i];
        result = result * (seq_t)10 + digit;
    }

    free(digits);
    return result;
}
