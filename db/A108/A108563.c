/*
 * A108563  Number of representations of n as sum of twice a square plus
 *          thrice a square.
 * https://oeis.org/A108563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to n = 2*a^2 + 3*b^2
 *            (counting all integer pairs (a,b), with signs counted
 *            separately)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 118 terms through n = 1000 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isqrt_u64.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* Maximum absolute value of a: a^2 <= n/2  */
    idx_t a_max = isqrt_u64(n / 2);
    for (idx_t a = 0; a <= a_max; ++a)
    {
        uint64_t aa = (uint64_t)a * a;          /* a^2 */
        if (2 * aa > n) break;                  /* safety guard */
        uint64_t remaining = n - 2 * aa;        /* 3*b^2 */
        if (remaining % 3 != 0) continue;
        uint64_t b2 = remaining / 3;            /* candidate b^2 */
        idx_t b = isqrt_u64(b2);
        if ((uint64_t)b * b != b2) continue;    /* not a perfect square */

        /* Number of b solutions for this a */
        seq_t bcnt = (b == 0) ? 1 : 2;
        /* Account for ±a, except a==0 */
        seq_t acnt = (a == 0) ? 1 : 2;
        count += acnt * bcnt;
    }
    return count;
}
