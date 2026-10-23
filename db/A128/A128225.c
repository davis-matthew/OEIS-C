/*
 * A128225  A127899 (unsigned) * A004736.
 * https://oeis.org/A128225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r*(2r-1) - (k-1)*2r where r = ceil( (sqrt(8n-7)-1)/2 )
 *            (the row containing index n) and k = n - r*(r-1)/2 (position
 *            within that row)
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the smallest r >= 1 with r*(r+1)/2 >= n.
   Uses binary search on idx_t, with intermediate products in
   unsigned __int128 to avoid overflow. */
static idx_t
find_row(idx_t n)
{
    idx_t low = 1, high = 1;
    /* grow high until it satisfies the inequality */
    while (((__uint128_t)high * (high + 1)) / 2 < n) {
        if (high > (idx_t)(-1) / 2) {      /* stop if high would overflow */
            high = n;
            break;
        }
        high <<= 1;
    }
    /* binary search between low and high (inclusive) */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t t = (__uint128_t)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)     /* outside the defined offset, behaviour undefined */
        return 0;

    /* Determine the row (r) and the position within the row (k). */
    idx_t r = find_row(n);

    /* offset = first index of row r = r*(r-1)/2 + 1   */
    idx_t offset = (idx_t)((( (__uint128_t)r * (r - 1)) / 2) + 1);
    idx_t k = n - offset + 1;            /* 1‑based column within the row */

    /* Core computation in seq_t. */
    seq_t rr = (seq_t)r;
    seq_t kk = (seq_t)(k - 1);           /* subtract 1 because k = 1 yields kk = 0 */

    /* Value = H(r) - (k-1)*2r where H(r) = r*(2r-1). */
    return rr * (2 * rr - 1) - kk * 2 * rr;
}
