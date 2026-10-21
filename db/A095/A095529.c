/*
 * A095529  a(n) = 68 written in base 13 - n.
 * https://oeis.org/A095529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = representation of decimal 68 in base (13 - n)
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_from_str.h"

seq_t a(idx_t n) {
    const idx_t value = 68;
    idx_t base = 13 - n;
    if (base <= 0) {
        /* Undefined for base <= 0; return 0. */
        return (seq_t)0;
    }
    if (base == 1) {
        /* Unary representation: 68 ones. */
        char buf[70];
        for (idx_t i = 0; i < value; ++i) buf[i] = '1';
        buf[value] = '\0';
        return seq_from_str(buf);
    }
    /* Base >= 2: compute digits in base 'base' and build decimal integer. */
    idx_t temp = value;
    unsigned char digits[10];
    idx_t len = 0;
    while (temp > 0) {
        digits[len++] = temp % base;
        temp /= base;
    }
    seq_t result = 0;
    for (idx_t i = len; i-- > 0;) {
        result = result * 10 + (seq_t)digits[i];
    }
    return result;
}
