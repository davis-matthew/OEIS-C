/*
 * A135222  Triangle A049310 + A000012 - I, read by rows.
 * https://oeis.org/A135222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) read by rows, * where (r,k) is chosen so that n =
 *            r*(r+1)/2 + k with r>=0, 0<=k<=r. * T(r,k) = 1 if r==k or (r-k)
 *            is odd; * = 1 + binomial((r+k)/2, (r-k)/2) otherwise
 * Range:     u8 n <= 126, u16 n <= 388, u32 n <= 1346, u64 n <= 4890,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(v)) for unsigned idx_t (uint64_t). */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = ((idx_t)1) << 32; /* hi is > sqrt(UINT64_MAX) */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __uint128_t sq = (__uint128_t)mid * (__uint128_t)mid;
        if (sq > v)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Determine row r so that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t t = (idx_t)(((__uint128_t)8 * n) + 1);          /* 8*n + 1 fits in 128 bits */
    idx_t s = isqrt_u64(t);                              /* floor(sqrt(8*n+1)) */
    idx_t r = (s - 1) / 2;                                /* row index */

    /* Number of elements before row r */
    idx_t prev = (idx_t)(((__uint128_t)r * (r + 1)) / 2);
    idx_t k = n - prev;                                   /* column index */

    /* Special case: diagonal always 1 */
    if (k == r)
        return (seq_t)1;

    idx_t diff = r - k;
    if (diff & 1) /* odd difference gives 1 */
        return (seq_t)1;

    /* Even difference: compute 1 + binomial((r+k)/2, (r-k)/2) */
    idx_t d = diff / 2;                /* (r-k)/2 */
    seq_t binom = 1;
    for (idx_t i = 1; i <= d; ++i) {
        binom = (binom * (seq_t)(k + i)) / (seq_t)i;
    }
    return binom + (seq_t)1;
}
