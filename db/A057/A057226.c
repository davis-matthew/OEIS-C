/*
 * A057226  Number of different symbols needed to express n as a Roman
 *          numeral.
 * https://oeis.org/A057226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of distinct Roman symbols used in n's Roman
 *            numeral
 * Range:     u8 n <= 3999, u16 n <= 3999, u32 n <= 3999, u64 n <= 3999,
 *            u128 n <= 3999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (extracted from A057226, classic Roman notation) */

#include <stdint.h>

static const idx_t val[13] = {1000, 900, 500, 400, 100, 90,
                              50, 40, 10, 9, 5, 4, 1};

static const uint8_t mask[13] = {0x40, 0x50, 0x20, 0x30,
                                 0x10, 0x14, 0x08, 0x0C,
                                 0x04, 0x05, 0x02, 0x03, 0x01};

seq_t a(idx_t n)
{
    idx_t remaining = n;
    uint8_t used = 0;

    for (int i = 0; i < 13; ++i) {
        if (remaining >= val[i]) {
            idx_t times = remaining / val[i];
            if (times) {
                used |= mask[i];
                remaining -= times * val[i];
            }
        }
    }

    /* Count set bits in used (max 7). */
    int count = 0;
    uint8_t m = used;
    while (m) {
        ++count;
        m &= m - 1;
    }
    return (seq_t)count;
}
