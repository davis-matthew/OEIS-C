/*
 * A191340  (A022839 mod 2)+(A108598 mod 2).
 * https://oeis.org/A191340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (floor(n*sqrt5) mod 2) + (floor(n*s) mod 2), where s =
 *            sqrt5/(sqrt5-1) = (5+sqrt5)/4
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 148 terms through n = 9822 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute floor(n*sqrt5) as floor(sqrt(5*n*n)).
   Then floor(n*s) = floor((5*n + floor(n*sqrt5))/4) using integer arithmetic. */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Convert n to seq_t */
    seq_t nn = (seq_t)n;

    /* floor(n*sqrt5) = floor(sqrt(5*n*n)) */
    seq_t sq = floor_sqrt((seq_t)5 * nn * nn);

    /* floor(n*s) = floor((5*n + sq)/4) */
    seq_t plus = (seq_t)5 * nn + sq;
    seq_t t = plus / (seq_t)4;

    /* Return number of odd elements: (sq mod 2) + (t mod 2) */
    seq_t res = (sq & 1) + (t & 1);
    return res;
}
