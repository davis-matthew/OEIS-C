/*
 * A021316  Decimal expansion of 1/312.
 * https://oeis.org/A021316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = 0 for n = 0 or 1, a(2) = 3; for n ≥ 3, a(n) =
 *            {2,0,5,1,2,8}[(n-3) mod 6]
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 3;
    static const seq_t pat[6] = {2, 0, 5, 1, 2, 8};
    return pat[(n - 3) % 6];
}
