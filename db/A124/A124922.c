/*
 * A124922  Second in a series of triangular arrays providing index numbers
 *          for subsequences of A060351.
 * https://oeis.org/A124922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn, tabl
 * Formula:   a(n) = 2^(r+1) + 3·2^t – 1, where r = ceil( (−1 + √(1+8n)) / 2
 *            ), t = n – r(r–1)/2 – 1 (zero‑based position in the row)
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* Integer square root of a 128‑bit value.
   Returns floor(√x).  */
static inline idx_t
floor_sqrt_u128(__int128 x)
{
    idx_t lo = 0, hi = ((idx_t)1 << 35);  /* 2^35 > √(2^67) */
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 0)           /* the offset is 1, so n=0 is never queried */
        return 0;

    /* Compute r = smallest integer with r(r+1)/2 ≥ n */
    __int128 t = (__int128)1 + (__int128)8 * n;
    idx_t s = floor_sqrt_u128(t);
    idx_t r = (s - 1) / 2;

    /* Adjust r in case the floor square‑root underestimates due to
       rounding in the binary‑search loop.  */
    while ((__int128)r * (r + 1) / 2 < n)
        r++;
    while (r > 0 && ((__int128)(r - 1) * r / 2) >= n)
        r--;

    /* Position within the row (zero‑based).  */
    idx_t pre = ((__int128)(r - 1) * r) / 2;
    idx_t tpos = n - pre - 1;  /* zero‑based offset inside the row */

    /* a(n) = 2^(r+1) + 3*2^t – 1  */
    seq_t val = pow2(r + 1) + 3 * pow2(tpos) - 1;
    return val;
}
