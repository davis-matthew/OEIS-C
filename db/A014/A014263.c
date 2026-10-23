/*
 * A014263  Numbers that contain even digits only.
 * https://oeis.org/A014263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal number obtained by writing (n‑1) in base‑5 and *
 *            multiplying each base‑5 digit by 2. This yields exactly all
 *            integers * whose decimal digits are all even
 * Range:     u8 n <= 40, u16 n <= 2250, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#define MAX_DIGITS 64

seq_t a(idx_t n)
{
    /* m = n‑1;  n ≥ 1, so m is always a valid unsigned */
    idx_t m = n - 1;
    /* store base‑5 digits least‑significant first */
    idx_t digits[MAX_DIGITS];
    size_t len = 0;

    if (m != 0) {
        while (m > 0) {
            digits[len++] = m % 5;
            m /= 5;
        }
    }
    /* construct the decimal number from most significant digit to least */
    seq_t res = 0;
    for (size_t i = len; i > 0; --i) {
        /* digits[i-1] ∈ [0,4]; multiplying by 2 gives an even decimal digit */
        res = res * 10 + (seq_t)(digits[i - 1]) * 2;
    }
    return res;           /* for n==1 this returns 0 as required */
}
