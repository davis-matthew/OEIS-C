/*
 * A095641  a(n) = 125 written in base 11 - n.
 * https://oeis.org/A095641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the base-(11-n) representation of decimal 125, viewed as
 *            a decimal number
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* base = 11 - n; use signed int to detect non‑positive bases */
    int64_t base_signed = (int64_t)11 - (int64_t)n;
    if (base_signed <= 0)
        return 0;                 /* undefined for n > 10 */

    idx_t base = (idx_t)base_signed;

    /* Unusual case: base 1 => unary representation of 125 (125 ones) */
    if (base == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < (idx_t)125; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* Convert decimal 125 to the chosen base.   */
    uint64_t m = 125ULL;
    uint8_t digits[40];
    idx_t len = 0;
    while (m > 0) {
        digits[len++] = (uint8_t)(m % base);
        m /= base;
    }

    /* Build the decimal number by concatenating digits in reverse order. */
    seq_t res = 0;
    for (idx_t i = len; i > 0; --i)
        res = res * 10 + digits[i - 1];

    return res;
}
