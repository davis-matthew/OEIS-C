/*
 * A167024  Triangle read by rows: T(n, m) = binomial(n, m)* Sum_{k=0..m}
 *          binomial(n, k) for 0 <= m <= n.
 * https://oeis.org/A167024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where * r = floor((sqrt(8*n+1) - 1)/2), * c = n -
 *            r*(r+1)/2, * T(r,c) = binomial(r,c) * sum_{k=0}^c binomial(r,k)
 * Range:     u8 n <= 17, u16 n <= 58, u32 n <= 178, u64 n <= 611,
 *            u128 n <= 2245, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static inline idx_t isqrt_u128(unsigned __int128 x)
{
    idx_t low = 0, high = (idx_t)1 << 32;               /* 2^32 is safely above sqrt(max) of 8*n+1 */
    while ((unsigned __int128)high * high <= x)
        high <<= 1U;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x)
            low = mid + 1;   /* mid fits, search higher */
        else
            high = mid;      /* mid too big, search lower */
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* first row */
        return binomial(0, 0);

    /* row r: r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    unsigned __int128 t   = (unsigned __int128)n * 8 + 1;
    idx_t          s     = isqrt_u128(t);
    idx_t          r     = (s - 1) / 2;

    /* ensure r is correct (guard against rounding errors) */
    unsigned __int128 start128 = (unsigned __int128)r * (r + 1) / 2;
    if (start128 > n) {
        --r;
        start128 = (unsigned __int128)r * (r + 1) / 2;
    }
    idx_t c = (idx_t)(n - (idx_t)start128);

    /* compute binomial(r,c) and the partial sum of binomials up to c */
    seq_t bin_c = binomial(r, c);
    seq_t sum   = 0;
    for (idx_t k = 0; k <= c; ++k)
        sum += binomial(r, k);

    return bin_c * sum;
}
