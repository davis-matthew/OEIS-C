/*
 * A095639  a(n) = 124 written in base 11 - n.
 * https://oeis.org/A095639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = representation of the decimal number 124 in base (11 -
 *            n), interpreted as a decimal integer
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The base in which 124 is expressed */
    int base = (int)(11ULL - n);
    /* The sequence is defined only for n = 0 … 10 (base ≥ 1). */
    if (n > 10 || base < 1) {
        return 0;            /* undefined for larger indices */
    }

    /* Special case: base 1 (unary) – 124 ones */
    if (base == 1) {
        seq_t res = 0;
        for (int i = 0; i < 124; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    /* Convert 124 to the given base and build the decimal number from its digits */
    int digits[32];
    int dlen = 0;
    int temp = 124;               /* constant value to convert */
    do {
        digits[dlen++] = temp % base;
        temp /= base;
    } while (temp > 0);

    seq_t res = 0;
    for (int i = dlen - 1; i >= 0; --i) {
        res = res * 10 + digits[i];
    }
    return res;
}
