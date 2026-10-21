/*
 * A095469  a(n) = 38 written in base 12 - n.
 * https://oeis.org/A095469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal integer formed by writing the decimal number 38
 *            in base (12 - n) and then interpreting that base‑12
 *            representation as a decimal number
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 10, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n >= 12) /* base (12 - n) would be <= 0, sequence not defined */
        return 0;

    idx_t base = 12 - n;        /* base ∈ {1,…,12} */

    /* case base >= 2 :  compute 38 in that base */
    if (base >= 2) {
        uint64_t val = 38;
        /* digits are small, at most 6 for base 12 */
        idx_t digits[8];
        idx_t len = 0;
        while (val > 0) {
            digits[len++] = val % base;
            val /= base;
        }
        seq_t res = 0;
        for (idx_t i = len; i > 0; --i) {
            res = res * 10 + digits[i - 1];
        }
        return res;
    }

    /* base == 1 : unary representation (38 ones) */
    seq_t res = 0;
    for (idx_t i = 0; i < 38; ++i)
        res = res * 10 + 1;
    return res;
}
