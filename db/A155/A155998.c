/*
 * A155998  Triangle read by rows: T(n, k) = f(n, k) + f(n, n-k), where f(n,
 *          k) = binomial(n, k)*(1 - (-1)^k)/2.
 * https://oeis.org/A155998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, T(r,k) = binomial(r,k) if r is odd; T(r,k) = 0 if r
 *            is even and k is even; T(r,k) = 2*binomial(r,k) if r is even and
 *            k is odd
 * Range:     u8 n <= 48, u16 n <= 179, u32 n <= 611, u64 n <= 2374,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial coefficient C(n,k) with minimal intermediate growth */
static seq_t binomial_func(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        result = result * (seq_t)(n - (k - i)); /* multiply first */
        result = result / (seq_t)i;            /* then divide */
    }
    return result;
}

/* Integer square root for 128‑bit values by binary search */
static idx_t isqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64) - 1;
    unsigned __int128 ans = 0;
    while (low <= high)
    {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq = mid * mid;
        if (sq <= x)
        {
            ans = mid;
            low = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return (idx_t)ans;
}

seq_t a(idx_t n)
{
    /* find row r */
    unsigned __int128 disc = (unsigned __int128)8 * n + 1;
    idx_t root = isqrt_uint128(disc);
    idx_t r = (root - 1) >> 1;          /* (root-1)/2 */

    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    seq_t bin = binomial_func(r, k);

    if (r & 1)            /* r odd: T = binomial(r,k) */
        return bin;
    if ((k & 1) == 0)     /* r even, k even: T = 0 */
        return (seq_t)0;
    return bin * (seq_t)2; /* r even, k odd: T = 2*binomial(r,k) */
}
