/*
 * A055477  Powers of ten written in base 6.
 * https://oeis.org/A055477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = decimal integer whose digits are the base‑6
 *            representation of 10^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^n.  Use the pow10 helper which works for all seq_t types. */
    seq_t x = pow10(n);

    /* Gather base‑6 digits of x (least‑significant first). */
    size_t cap = 32;
    unsigned char *digits = (unsigned char *)malloc(cap * sizeof(unsigned char));
    if (!digits) return 0;               /* OOM guard – unlikely in tests */
    size_t len = 0;

    while (x > 0) {
        seq_t r = x % (seq_t)6;
        if (len == cap) {
            cap <<= 1;                   /* double capacity */
            unsigned char *tmp = (unsigned char *)realloc(digits, cap * sizeof(unsigned char));
            if (!tmp) {
                free(digits);
                return 0;
            }
            digits = tmp;
        }
        digits[len++] = (unsigned char)r;
        x = x / (seq_t)6;
    }

    /* Construct the result from most‑significant digit to least‑significant. */
    seq_t result = 0;
    const seq_t ten = (seq_t)10;
    for (size_t i = 0; i < len; ++i) {
        result = result * ten + (seq_t)digits[len - 1 - i];
    }

    free(digits);
    return result;
}
