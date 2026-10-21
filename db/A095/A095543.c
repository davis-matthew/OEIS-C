/*
 * A095543  a(n) = 75 written in base 12 - n.
 * https://oeis.org/A095543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = decimal value of 75 written in base (12‑n) */
#include <stdint.h>

seq_t a(idx_t n)
{
    int64_t base = 12 - (int64_t)n;

    if (base >= 2) {
        uint64_t val = 75;
        uint8_t digits[12];
        int  len = 0;
        uint64_t b = (uint64_t)base;

        /* convert 75 to base `base` */
        while (val > 0) {
            digits[len++] = (uint8_t)(val % b);
            val /= b;
        }

        /* interpret the digits as a decimal integer */
        seq_t res = 0;
        for (int i = len - 1; i >= 0; --i) {
            res = res * 10 + (seq_t)digits[i];
        }
        return res;
    } else if (base == 1) {
        /* unary representation: 75 ones */
        seq_t res = 0;
        for (idx_t i = 0; i < 75; ++i) {
            res = res * 10 + 1;
        }
        return res;
    } else {
        /* base <= 0 is not defined for this sequence */
        return 0;
    }
}
