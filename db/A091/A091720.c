/*
 * A091720  Babylonian sexagesimal (base 60) expansion of 1/7.
 * https://oeis.org/A091720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 8,34,17 repeating (a(0)=8, a(1)=34, a(2)=17)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    static const seq_t pat[3] = {8, 34, 17};
    return pat[(size_t)(n % 3)];
}
