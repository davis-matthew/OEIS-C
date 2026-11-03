/*
 * A124819  Number triangle T(n,k)=C(n+2k+1,3k+1).
 * https://oeis.org/A124819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binom(r + 2k + 1, 3k + 1) where r =
 *            floor((sqrt(8n+1)-1)/2) (row index) and k = n - r(r+1)/2
 *            (position in the row)
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 52 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
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
    /* compute 8*n+1 as a seq_t to avoid overflow in seq_t operations */
    seq_t tmp = (seq_t)(8 * n + 1);
    seq_t root = floor_sqrt(tmp);             /* floor(sqrt(8n+1)) */

    /* r = (sqrt(8n+1) - 1) / 2  (integer division truncates) */
    idx_t r = (idx_t)((root - 1) / 2);

    /* number of elements before row r */
    idx_t prefix = r * (r + 1) / 2;

    /* position k within the row */
    idx_t k = n - prefix;

    /* compute the binomial coefficient C(r + 2k + 1, 3k + 1) */
    return binomial(r + 2 * k + 1, 3 * k + 1);
}
