/*
 * A027663  a(n)/100000 gives sqrt(n) to 5 places after the decimal point.
 * https://oeis.org/A027663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = round(sqrt(n) * 100000)
 * Range:     u8 none, u16 none, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <inttypes.h>

/* Integer square root for a 128‑bit unsigned integer.
   Returns floor(sqrt(x)).  */
static inline uint64_t uint128_sqrt(unsigned __int128 x)
{
    uint64_t lo = 0, hi = UINT64_MAX, mid, ans = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;  /* avoid underflow */
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Compute n * 10^10 using 128‑bit arithmetic to avoid overflow. */
    const unsigned __int128 SCALE = (unsigned __int128)10000000000ULL;  /* 10^10 = 100000^2 */
    unsigned __int128 prod = (unsigned __int128)n * SCALE;

    /* Floor sqrt of prod. */
    uint64_t m = uint128_sqrt(prod);

    /* Determine whether to round up.
       Condition: if prod - m^2 >= m + 1  then m++  */
    unsigned __int128 diff = prod - (unsigned __int128)m * m;
    if (diff >= (unsigned __int128)m + 1) {
        m++;
    }

    return (seq_t)m;
}
