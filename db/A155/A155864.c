/*
 * A155864  Triangle T(n,k) = n*(n-1)*binomial(n-2, k-1) for 1 <= k <= n-1, n
 *          >= 2, and T(n,0) = T(n,n) = 1 for n >= 0, read by rows.
 * https://oeis.org/A155864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n + 1) - 1) / 2), k = n -
 *            r*(r+1)/2, T(r,0)=T(r,r)=1, and for 1 <= k <= r-1 T(r,k) =
 *            r*(r-1)*binomial(r-2, k-1)
 * Range:     u8 n <= 30, u16 n <= 96, u32 n <= 417, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_n_k.h"

static idx_t locate_row(idx_t n)
{
    idx_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= n) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    idx_t r = locate_row(n);
    __uint128_t start = (__uint128_t)r * (r + 1) / 2;
    idx_t k = (idx_t)(n - start);

    if (k == 0 || k == r) {
        return (seq_t)1;
    }

    seq_t prod1 = (seq_t)r * (seq_t)(r - 1);          /* r*(r-1) */
    seq_t binom = binom_n_k(r - 2, k - 1);             /* binomial(r-2, k-1) */
    return prod1 * binom;
}
