/*
 * A035510  2nd column of Inverse Stolarsky array.
 * https://oeis.org/A035510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n*φ) + 1 + floor((round(n*φ)+1/2)*φ) where φ =
 *            (1+√5)/2. The computation uses only integer arithmetic and the
 *            provided floor_sqrt function
 * Range:     u8 n <= 0, u16 n <= 17, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 57 (2026-11-01)
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
    if (n == 0)
        return 4;                          /* offset 0 */

    seq_t nn   = (seq_t)n;                  /* cast index to seq_t */
    /* floor(n*√5) */
    seq_t fsqrt5_n = floor_sqrt((seq_t)5 * nn * nn);

    /* round(n*φ) = floor( (n + sqrt5_n + 1)/2 ) */
    seq_t round_phi = (nn + fsqrt5_n + 1) / 2;

    /* t = 2*round_phi + 1 */
    seq_t t = 2 * round_phi + 1;

    /* floor(t*√5) */
    seq_t fsqrt5_t = floor_sqrt((seq_t)5 * t * t);

    /* floor((t + t*√5)/4) = floor((t + fsqrt5_t)/4) */
    seq_t floor_part = (t + fsqrt5_t) / 4;

    return round_phi + 1 + floor_part;
}
