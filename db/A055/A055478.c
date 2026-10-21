/*
 * A055478  Powers of ten written in base 7.
 * https://oeis.org/A055478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = decimal value of the base‑7 representation of 10^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* 10ⁿ as a big integer. */
    seq_t pow10 = seq_pow((seq_t)10, n);

    /* Convert to base‑7 digits (least‑significant first). */
    size_t cap = n * 2 + 10;                /* generous initial capacity */
    unsigned char *digits = (unsigned char *)malloc(cap);
    if (!digits) return 0;                  /* out of memory → zero */

    size_t len = 0;
    while (pow10 != 0) {
        seq_t rem = pow10 % (seq_t)7;        /* base‑7 digit */
        digits[len++] = (unsigned char)rem;
        pow10 = pow10 / (seq_t)7;

        if (len == cap) {
            cap *= 2;
            unsigned char *tmp = (unsigned char *)realloc(digits, cap);
            if (!tmp) { free(digits); return 0; }
            digits = tmp;
        }
    }

    /* Build the decimal number from the extracted digits. */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i) {
        result = result * (seq_t)10 + (seq_t)digits[len - 1 - i];
    }

    free(digits);
    return result;
}
