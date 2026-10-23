/*
 * A131413  Triangle read by rows: A002024 + A128076 - A000012 as infinite
 *          lower triangular matrices.
 * https://oeis.org/A131413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 3*row + 1 - pos , where row = floor((sqrt(8*n+1)-1)/2)
 *            and pos = n - row*(row+1)/2
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of an unsigned 128-bit value */
static uint64_t _isqrt_u128(unsigned __int128 x)
{
    uint64_t lo = 0, hi = UINT64_MAX, mid, ans = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* compute the row index r */
    unsigned __int128 t = (unsigned __int128)n * 8 + 1;
    uint64_t s = _isqrt_u128(t);
    uint64_t r = (s - 1) / 2;

    /* start index of row r */
    unsigned __int128 offset128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t offset = (idx_t)offset128;

    /* position within the row (0-based) */
    idx_t pos = n - offset;

    /* compute the value */
    seq_t value = (seq_t)( (seq_t)(3 * r + 1) - pos );
    return value;
}
