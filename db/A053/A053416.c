/*
 * A053416  Circle numbers (version 4): a(n)= number of points
 *          (i+j/2,j*sqrt(3)/2), i,j integers (triangular grid) contained in a
 *          circle of diameter n, centered at (0,0).
 * https://oeis.org/A053416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{(i,j) ∈ ℤ² | i² + i*j + j² ≤ n²/4}
 * Range:     u8 n <= 17, u16 n <= 268, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 67 terms through n = 951 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the lattice points of the triangular lattice inside a circle
   of diameter n, centered at the origin.
   The implementation runs in O(n) time and uses only integer arithmetic.
   It relies on the helper function _isqrt_u64 from “../utils/_isqrt_u64.h”.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/isqrt.h"

static inline int64_t ceil_div2_int64(int64_t a)
{
    /* ceil(a/2) for signed 64‑bit integer a */
    return (a >= 0) ? (a + 1) / 2 : a / 2;
}

static inline int64_t floor_div2_int64(int64_t a)
{
    /* floor(a/2) for signed 64‑bit integer a */
    return (a >= 0) ? a / 2 : (a - 1) / 2;
}

seq_t a(idx_t n)
{
    idx_t n2 = n * n;                 /* n² ≤ 10⁸ for the test constraints  */
    int64_t jmax = (int64_t)(isqrt(n2 / 3)); /* floor(n/√3) */
    unsigned long long sum = 0;        /* accumulates the result, fits in 64‑bit */

    for (int64_t j = -jmax; j <= jmax; ++j)
    {
        unsigned long long jj = (unsigned long long)(j >= 0 ? j : -j);
        /* m = n² - 3·j² as 128‑bit to avoid overflow */
        unsigned __int128 temp = (unsigned __int128)n * n;
        temp -= (unsigned __int128)3 * (unsigned __int128)jj * jj;
        idx_t m = (idx_t)temp;        /* m ≥ 0 for |j| ≤ jmax */
        idx_t sqrt_m = isqrt(m);

        int64_t A = -j - (int64_t)sqrt_m;   /* -j - √m   */
        int64_t B = -j + (int64_t)sqrt_m;   /* -j + √m   */

        int64_t min_i = ceil_div2_int64(A);
        int64_t max_i = floor_div2_int64(B);

        /* number of integers i in the interval [min_i, max_i] */
        if (max_i >= min_i)
            sum += (unsigned long long)(max_i - min_i + 1);
    }

    return (seq_t)sum;
}
