/*
 * A119963  Triangle T(n,k), 0 <= k <= n, read by rows, with T(2n,2k) =
 *          T(2n+1,2k) = T(2n+1,2k+1) = T(2n+2,2k+1) = binomial(n,k).
 * https://oeis.org/A119963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r(r+1)/2, T(r,k) = binomial(floor(r/2) - (k mod 2)*(1-(r mod
 *            2)), floor(k/2))
 * Range:     u8 n <= 259, u16 n <= 610, u32 n <= 1274, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Integer square root for unsigned 64‑bit numbers.
   Returns floor(sqrt(n)). */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low  = 0;
    idx_t high = ((idx_t)1 << 32);
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;   /* high is floor(sqrt(n)) after loop */
}

seq_t a(idx_t n)
{
    /* determine row r */
    idx_t m = 8 * n + 1;
    idx_t s = isqrt_u64(m);
    idx_t r = (s - 1) >> 1;          /* floor((sqrt(8n+1)-1)/2) */

    /* column k within row r */
    unsigned __int128 start = (unsigned __int128)r * (r + 1) / 2;  /* first index of row r */
    idx_t k = n - (idx_t)start;

    /* parameters for binomial */
    idx_t r_half = r >> 1;
    idx_t sub    = (k & 1) * (1 - (r & 1));
    idx_t n_choose = r_half - sub;
    idx_t k_choose = k >> 1;

    return binomial(n_choose, k_choose);
}
