/*
 * A096921  Triangle array of binomial coefficients.
 * https://oeis.org/A096921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(floor((r+k)/2), floor(k/2)) * where r =
 *            floor((sqrt(1+8n)-1)/2) and k = n - r*(r+1)/2
 * Range:     u8 n <= 43, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = (idx_t)1ULL << 32; /* sqrt(UINT64_MAX) < 2^32 */
    while (low <= high)
    {
        idx_t mid = (low + high) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq == n)
            return mid;
        else if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high; /* floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    /* row index r */
    idx_t r = (isqrt_u64(1 + 8 * n) - 1) / 2;

    /* starting index of row r */
    idx_t start = (idx_t)(((__int128)r * (r + 1)) / 2);

    /* column index k */
    idx_t k = n - start;

    /* binomial parameters */
    idx_t m  = (r + k) / 2;   /* floor((r + k)/2) */
    idx_t kk = k / 2;         /* floor(k/2) */

    return binomial(m, kk);
}
