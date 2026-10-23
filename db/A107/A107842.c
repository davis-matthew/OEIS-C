/*
 * A107842  A number triangle of lattice walks.
 * https://oeis.org/A107842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (3k+2) * C(2r+k+1, r-k) / (r+2k+2)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where r = floor((sqrt(8n+1)-1)/2) and k = n - r(r+1)/2 */
#include "../utils/binom.h"
#include <stdint.h>

/* integer square root (floor) for unsigned 64‑bit integers */
static idx_t isqrt_idx(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* compute row r */
    unsigned __int128 tmp = (unsigned __int128)8 * n + 1;
    idx_t r = (isqrt_idx((idx_t)tmp) - 1) / 2;

    /* compute column k */
    idx_t row_start = r * (r + 1) / 2;
    idx_t k = n - row_start;

    /* binomial coefficient C(2r+k+1, r-k) */
    idx_t N = 2 * r + k + 1;
    idx_t K = r - k;
    seq_t c = binom(N, K);

    /* numerator and denominator of the formula */
    seq_t denom = (seq_t)(r + 2 * k + 2);
    seq_t three_k_plus_two = (seq_t)(3 * k + 2);

    /* perform division safely to avoid overflow in intermediate
       multiplication when seq_t is 64 bits */
    seq_t base = c / denom;
    seq_t rem  = c % denom;
    seq_t result = base * three_k_plus_two + (rem * three_k_plus_two) / denom;

    return result;
}
