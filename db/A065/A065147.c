/*
 * A065147  100 written in base 15-n.
 * https://oeis.org/A065147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, less, fini, full, nonn
 * Formula:   a(n) = representation of decimal 100 in base (15 - n)
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For base >= 2, the digits of 100 in that base are concatenated in decimal form. */
/* For base 1, the representation is 100 repetitions of the digit '1' (unary). */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t base = 15 - n;
    if (base <= 0) return 0;          /* undefined for n >= 15 */

    if (base == 1) {                  /* unary representation: 100 ones */
        seq_t res = 0;
        for (idx_t i = 0; i < 100; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* convert 100 into the given base */
    seq_t value = 100;                /* the number to convert */
    unsigned char digits[20]; /* enough for any reasonable base < 15 */
    int len = 0;
    while (value > 0) {
        digits[len++] = (unsigned char)(value % base);
        value = value / base;
    }

    /* assemble the decimal number from the digits */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i)
        result = result * 10 + digits[i];

    return result;
}
