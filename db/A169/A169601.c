/*
 * A169601  a(n) = (10^n - 1)^2 + 2.
 * https://oeis.org/A169601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n - 1)^2 + 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* t = 10^n − 1 */
    seq_t t = pow10(n);
    t -= 1;
    /* res = t^2 + 2 */
    seq_t res = t * t;
    res += 2;
    return res;
}
