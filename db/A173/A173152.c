/*
 * A173152  Triangle T(n,m) read by rows: T(n,0)=T(n,n)=1, else T(n,m) =
 *          binomial(n,m) + 2^(2*n-3)*binomial(n-2,m-1).
 * https://oeis.org/A173152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   a(n) = T(r, m) where * r = floor((sqrt(8n+1)-1)/2), m = n -
 *            r(r+1)/2; * T(r,0)=T(r,r)=1, otherwise * T(r,m)=binomial(r,m) +
 *            2^(2r-3) * binomial(r-2,m-1)
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* Integer square root: floor(sqrt(x)) */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x, res = 0;
    while (lo <= hi)
    {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
        {
            res = mid;
            lo = mid + 1;
        }
        else
            hi = mid - 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find row r and column m for index n */
    idx_t r;
    if (n == 0) {
        r = 0;
    } else {
        /* r = floor((sqrt(8*n + 1) - 1) / 2) */
        idx_t tmp = isqrt_u64(8 * n + 1);
        r = (tmp - 1) / 2;
    }

    idx_t offset = r * (r + 1) / 2;   /* linear index where row r starts */
    idx_t m = n - offset;

    if (m == 0 || m == r) {
        return (seq_t)1;
    }

    seq_t bin1 = binomial(r, m);
    seq_t bin2 = binomial(r - 2, m - 1);
    /* power of two: 2^(2r-3) */
    seq_t pow_term = pow2(2 * r - 3);

    return bin1 + pow_term * bin2;
}
