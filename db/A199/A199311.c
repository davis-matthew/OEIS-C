/*
 * A199311  a(n) = 8*5^n+1.
 * https://oeis.org/A199311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*5^n+1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t power = 1;   /* 5^0 */
    seq_t base  = 5;   /* 5^1 */
    idx_t e = n;

    while (e) {
        if (e & 1)
            power *= base;
        base *= base;
        e >>= 1;
    }

    return power * 8 + 1;
}
