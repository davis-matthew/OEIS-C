/*
 * A095547  a(n) = 77 written in base 12 - n.
 * https://oeis.org/A095547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal integer formed by writing 77 in base (12 - n)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* base = 12 - n */
    if (n > 12) {
        /* sequence is defined only for n = 0 … 11 */
        return (seq_t)0;
    }
    idx_t base = (idx_t)12 - n;
    if (base == 0) {
        return (seq_t)0;
    }

    /* base 1: unary representation of 77 */
    if (base == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 77; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    /* compute digits of 77 in base `base` (digits are 0‑9 only) */
    idx_t val = 77;
    idx_t digits[32];
    size_t cnt = 0;
    while (val > 0) {
        digits[cnt++] = val % base;
        val = val / base;
    }

    /* convert digits to decimal integer */
    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i) {
        res = res * 10 + digits[cnt - 1 - i];
    }
    return res;
}
