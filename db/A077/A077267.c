/*
 * A077267  Number of zeros in base-3 expansion of n.
 * https://oeis.org/A077267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   number of zeros in base-3 expansion of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t cnt = 0;
    while (n > 0) {
        if (n % 3 == 0) cnt++;
        n /= 3;
    }
    return cnt;
}
