/*
 * A095579  a(n) = 93 written in base 15 - n.
 * https://oeis.org/A095579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = digits of the decimal number 93 expressed in base (15-n),
 *            interpreted as a decimal integer. For n >= 15 the value is
 *            undefined (return 0)
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* base = 15 - n must be positive */  
    if (n >= 15) {
        return (seq_t)0;
    }
    idx_t base = (idx_t)15 - n;
    const idx_t value = 93;   /* constant to convert */

    /* Handle base 1 (unary) separately */
    if (base == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < value; ++i) {   /* 93 digits of 1 */
            res = res * (seq_t)10 + (seq_t)1;
        }
        return res;
    }

    /* Convert 93 to the given base to obtain its digits */
    idx_t tmp = value;
    int digits[32];   /* maximum digits needed are <= 32 (93 fits) */
    int len = 0;
    while (tmp > 0) {
        digits[len++] = (int)(tmp % base);
        tmp /= base;
    }

    /* Reverse digits to obtain most‑significant first */
    seq_t result = (seq_t)0;
    for (int i = len - 1; i >= 0; --i) {
        result = result * (seq_t)10 + (seq_t)digits[i];
    }
    return result;
}
