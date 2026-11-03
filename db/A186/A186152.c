/*
 * A186152  Rank of (1/8)n^3 when {(1/8)i^3: i>=1} and {j^2>: j>=1} are
 *          jointly ranked with (1/8)i^3 before j^2 when (1/8)i^3=j^2.
 *          Complement of A186153.
 * https://oeis.org/A186152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor( sqrt((1/8)*n^3 - 1/16) )
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    if (n == 0)   /* offset is 1, but return 0 for safety */
        return 0;

    seq_t nn   = (seq_t)n;          /* convert index to seq_t   */
    seq_t cube = nn * nn * nn;      /* n^3                       */
    seq_t two_cube = cube + cube;   /* 2*n^3                     */
    seq_t tmp      = two_cube - 1;  /* 2*n^3 - 1                 */

    seq_t sqrt_val = floor_sqrt(tmp); /* floor(sqrt(2*n^3-1))    */
    seq_t add      = sqrt_val / 4;    /* floor of sqrt_val / 4   */

    return nn + add;                 /* final value a(n)        */
}
