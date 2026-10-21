/*
 * A095608  a(n) = 109 written in base n.
 * https://oeis.org/A095608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = digits of decimal 109 written in base n, interpreted as a
 *            decimal integer (concatenation of the base‑n digits)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base‑1 is unary: 109 ones */
    if (n <= 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 109; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    /* Compute the digits of 109 in base n (reverse order). */
    uint64_t val = 109;
    uint8_t digits[32];            /* enough for any n≥2 */
    int dcnt = 0;
    uint64_t base = (uint64_t)n;

    while (val > 0) {
        digits[dcnt++] = val % base;
        val /= base;
    }

    /* Convert the digit sequence into a decimal integer. */
    seq_t res = 0;
    for (int i = dcnt - 1; i >= 0; --i) {
        uint64_t d = digits[i];
        if (d < 10) {
            res = res * 10 + d;
        } else if (d < 100) {
            res = res * 100 + d;
        } else {            /* d is at most 109 for this sequence */
            res = res * 1000 + d;
        }
    }
    return res;
}
