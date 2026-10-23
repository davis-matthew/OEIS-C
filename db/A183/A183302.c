/*
 * A183302  Complement of A014106.
 * https://oeis.org/A183302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + m if n >= 2*m*(m+1)+1 n + m - 1 otherwise where m =
 *            floor(sqrt(n/2))
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: floor(sqrt(x)) for 64‑bit unsigned x */
static idx_t floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 32;            /* hi^2 fits in 128 bits */
    while (lo <= hi) {
        idx_t mid  = lo + (hi - lo) / 2;
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    idx_t half = n / 2;                   /* integer division == floor(n/2) */
    idx_t m = floor_sqrt_u64(half);

    /* Compute T = 2*m*(m+1)+1 using 128‑bit arithmetic to avoid overflow */
    __int128 tmp = (__int128)m * (m + 1);
    idx_t T = (idx_t)(tmp * 2 + 1);

    seq_t ans = (seq_t)n;
    seq_t mseq = (seq_t)m;

    if (n >= T)
        ans += mseq;
    else
        ans += (mseq - 1);

    return ans;
}
