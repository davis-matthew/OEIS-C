/*
 * A188794  a(n) is the smallest integer k >= 2 such that the number of
 *          divisors d>1 of n-k with k|(n-d) equals A188550(n).
 * https://oeis.org/A188794
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 9964, u16 n <= 9964, u32 n <= 9964, u64 n <= 9964,
 *            u128 n <= 9964, big any n
 * Helpers:   count_congruent_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A188794: a(n) is the smallest integer k >= 2 such that the
 * number of divisors d>1 of n-k with k | (n-d) equals A188550(n).
 * For each n >= 4, we search k from 2 up to ⌊√n⌋, compute the count
 * of such divisors and take the minimal k giving the maximum count. */

#include <stdint.h>
#include "../utils/count_congruent_divisors.h"

/* integer square‑root floor (compatible with C11 and C++17) */
static idx_t floor_isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high)
    {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)
        {
            low = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;                 /* offset: sequence starts at n = 4 */

    idx_t limit   = floor_isqrt_u64(n);
    idx_t bestK   = 0;                   /* will be set in the loop */
    seq_t bestCnt = 0;

    for (idx_t k = 2; k <= limit; ++k)
    {
        seq_t cnt = count_congruent_divisors(n - k, k);
        if (cnt > bestCnt || (cnt == bestCnt && k < bestK))
        {
            bestCnt = cnt;
            bestK   = k;
        }
    }

    return (seq_t)bestK;
}
