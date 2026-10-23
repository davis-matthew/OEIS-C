/*
 * A120027  Triangle, generated from (3^(n-k) * 5^k) table.
 * https://oeis.org/A120027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 3^i * 5^j, where d = ⌊(√(8 n + 1) − 1)/2⌋, k = n − d(d +
 *            1)/2, i = d − k, j = k. (Antidiagonals of the 3^i·5^j table,
 *            read bottom‑up on each diagonal.)
 * Range:     u8 n <= 12, u16 n <= 34, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   pow3, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow3.h"
#include "../utils/power5.h"

/* integer square root of an unsigned __int128 value */
static inline idx_t isqrt128(unsigned __int128 x)
{
    idx_t lo = 0, hi = 1;
    while ((unsigned __int128)hi * hi <= x) hi <<= 1;
    while (lo + 1 < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * mid <= x) lo = mid;
        else hi = mid;
    }
    return lo;
}

/* compute A120027(n) */
seq_t a(idx_t n)
{
    /* compute d = floor((√(8n+1)−1)/2) */
    unsigned __int128 s = (unsigned __int128)8 * n + 1;
    idx_t sqrt_s = isqrt128(s);
    idx_t d = (sqrt_s - 1) / 2;

    /* local index k on diagonal d */
    idx_t prev_total = (idx_t)((unsigned __int128)d * (d + 1) / 2);
    idx_t k = n - prev_total;

    idx_t i = d - k; /* exponent of 3 */
    idx_t j = k;     /* exponent of 5 */

    return pow3(i) * power5(j);
}
