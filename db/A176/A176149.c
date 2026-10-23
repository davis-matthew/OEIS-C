/*
 * A176149  Triangle T(n, k) = n + 1 - mod(n, k+1) - mod(n, n-k+1), read by
 *          rows.
 * https://oeis.org/A176149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where * r = floor((sqrt(8*n+1)-1)/2) * k = n -
 *            r*(r+1)/2 * and T(r,k) = r+1 - mod(r,k+1) - mod(r,r-k+1) (modulo
 *            is C's %) * This implements the OEIS sequence A176149
 * Range:     u8 n <= 5150, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root of a 128‑bit number.
 * Returns floor(sqrt(x)).  x is guaranteed to fit in 128 bits. */
static idx_t isqrt_uint128(unsigned __int128 x)
{
    idx_t low = 0;
    /* upper bound: sqrt(8*2^64) < 2^34, use 1<<35 for safety */
    idx_t high = (idx_t)1ULL << 35;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* compute r = floor((sqrt(8*n+1)-1)/2) */
    unsigned __int128 x = (unsigned __int128)8 * n + 1;
    idx_t s = isqrt_uint128(x);
    idx_t r = (s - 1) / 2;

    /* offset = r*(r+1)/2 */
    unsigned __int128 offset128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t offset = (idx_t)offset128;

    idx_t k = n - offset;                /* position within the row */

    idx_t mod1 = r % (k + 1);
    idx_t mod2 = r % (r - k + 1);

    seq_t val = (seq_t)(r + 1 - mod1 - mod2);
    return val;
}
