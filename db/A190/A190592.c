/*
 * A190592  Maximal digit in base-3 expansion of n.
 * https://oeis.org/A190592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximal base-3 digit of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    bool has1 = false;
    while (n > 0) {
        idx_t r = n % 3;
        if (r == 2)
            return 2;
        if (r == 1)
            has1 = true;
        n /= 3;
    }
    return has1 ? 1 : 0;
}
