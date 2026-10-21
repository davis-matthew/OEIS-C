/*
 * A096894  Least inradius of primitive Pythagorean triangles with odd leg
 *          2n+1.
 * https://oeis.org/A096894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min_{d|(2n+1), d < (2n+1)/d, gcd(d,(2n+1)/d)=1}
 *            d*((2n+1)/d - d)/2
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   _floor_isqrt_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* m = 2n+1 (always odd) */
    idx_t m = n * 2 + 1;
    /* candidate from d = 1 gives r = n */
    seq_t min_val = (seq_t)n;

    idx_t sqrt_m = _floor_isqrt_u64(m);
    for (idx_t d = 1; d <= sqrt_m; ++d) {
        if (m % d != 0) continue;
        idx_t e = m / d;
        if (e <= d) continue;
        if (gcd((seq_t)d, (seq_t)e) != 1) continue;
        seq_t r = ((seq_t)d * ((seq_t)e - (seq_t)d)) / 2;
        if (r < min_val) min_val = r;
    }
    return min_val;
}
