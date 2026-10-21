/*
 * A098157  Triangle T(n,k) with diagonals T(n,n-k) = binomial(n+1,2k).
 * https://oeis.org/A098157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = Flattened triangle T(r,c) with T(r,c) = binomial(r+1,
 *            2*(r-c)), r,c >= 0, 0 <= c <= r. Row r contains r+1 terms; the
 *            sequence is the row-major listing of the triangle
 * Range:     u8 n <= 32, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Locate the row r such that cumulative count up to r exceeds n. */
    idx_t low = 0;
    idx_t high = (idx_t)UINT64_MAX;          /* upper bound for any idx_t */

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t cum = ( (__uint128_t)(mid + 1) * (mid + 2) ) / 2;
        if (cum > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Number of terms preceding row r. */
    __uint128_t prev = ( (__uint128_t)r * (r + 1) ) / 2;
    idx_t c = n - (idx_t)prev;               /* column index within row r */

    idx_t j = r - c;                         /* r - c */
    idx_t a_param = r + 1;                   /* n + 1 for the binomial */
    idx_t b_param = j * 2;                   /* 2*(r-c) */

    if (b_param > a_param)                   /* binom will return 0, guard */
        return 0;

    return binom(a_param, b_param);
}
