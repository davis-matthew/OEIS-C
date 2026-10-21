/*
 * A220272  Number of ways to write n=x^2+y (x>0, y>0) with 2*x*y-1 prime.
 * https://oeis.org/A220272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs of positive integers (x, y) such that n =
 *            x^2 + y and 2*x*y - 1 is prime
 * Range:     u8 n <= 49, u16 n <= 2944, u32 n <= 9661, u64 n <= 9661,
 *            u128 n <= 9661, big any n
 * Helpers:   _floor_isqrt_u64, isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/isPrimeTrial.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    seq_t count = 0;
    idx_t maxx = _floor_isqrt_u64(n - 1);        /* largest x with x^2 < n   */
    for (idx_t x = 1; x <= maxx; ++x) {
        idx_t y = n - x * x;                     /* y > 0 automatically   */
        seq_t p = (seq_t)2 * (seq_t)x * (seq_t)y - 1;
        if (isPrimeTrial(p)) ++count;
    }
    return count;
}
