/*
 * A095581  a(n) = 94 written in base 11 - n.
 * https://oeis.org/A095581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal number formed by the digits of the decimal * 94
 *            expressed in base (11 - n). For bases 11,10,…,2 the digits are *
 *            86,94,114,136,163,234,334,1132,10111,1011110. For base 1 a(10) *
 *            is the 94‑digit number 111…1 (94 ones)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* base = 11 - n ; only defined for n ≤ 10 */
    int base = 11 - (int)n;
    if (base <= 0)
        return (seq_t)0;              /* undefined for n > 10 */

    /* base 1: produce 94 ones */
    if (base == 1) {
        seq_t res = 0;
        for (int i = 0; i < 94; ++i)
            res = res * (seq_t)10 + (seq_t)1;
        return res;
    }

    /* base 2..10 : convert 94 to chosen base, then build decimal value */
    int value = 94;
    int digits[10];   /* 94 in base 2 needs at most 7 digits */
    int m = 0;
    while (value > 0) {
        digits[m++] = value % base;
        value /= base;
    }

    seq_t res = 0;
    for (int i = m - 1; i >= 0; --i)
        res = res * (seq_t)10 + (seq_t)digits[i];

    return res;
}
