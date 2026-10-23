/*
 * A021241  Decimal expansion of 1/237.
 * https://oeis.org/A021241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth digit of the decimal expansion of 1/237
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    const idx_t denom = 237;
    idx_t r = 1;   /* initial remainder before first decimal digit */
    for (idx_t i = 0; i <= n; ++i) {
        idx_t prod = r * 10;
        seq_t digit = (seq_t)(prod / denom);
        r = prod % denom;
        if (i == n)
            return digit;
    }
    /* unreachable */
    return (seq_t)0;
}
