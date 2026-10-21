/*
 * A202750  Triangle T(n,k) = binomial(n,k)^4 read by rows, 0<=k<=n.
 * https://oeis.org/A202750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = binomial(r,k)^4 where r = floor((sqrt(8*n+1)-1)/2), k = n
 *            - r*(r+1)/2 (binomial(r,k) is taken over integers)
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 69, u64 n <= 197,
 *            u128 n <= 230, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include <math.h>           /* sqrtl */

static inline idx_t
find_row(idx_t n)
{
    /* Find largest r with r*(r+1)/2 <= n using binary search */
    idx_t low  = 0;
    /* upper bound: sqrt(2*n)+2, use long double for safety */
    idx_t high = (idx_t)sqrtl((long double)n * 2.0L + 1.0L) + 2;

    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t
a(idx_t n)
{
    idx_t r   = find_row(n);
    unsigned __int128 cum = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)cum;

    seq_t binom = binomial(r, k);
    seq_t res   = binom;
    res *= binom;
    res *= binom;
    res *= binom;
    return res;
}
