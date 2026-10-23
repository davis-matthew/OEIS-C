/*
 * A159637  Start with [0], repeatedly apply the map 0 -> [01/10], 1 ->
 *          [10/01].
 * https://oeis.org/A159637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = popcount(k) XOR popcount(d-k) where d =
 *            floor((sqrt(8*n+1)-1)/2) (i.e. largest d with d*(d+1)/2 ≤ n) and
 *            k = n - d*(d+1)/2. This yields the antidiagonal reading of the
 *            square array T(i,j) = Thue‑Morse(i) XOR Thue‑Morse(j)
 * Range:     u8 n <= 9678, u16 n <= 9678, u32 n <= 9678, u64 n <= 9678,
 *            u128 n <= 9678, big any n
 * Helpers:   parity_popcnt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/parity_popcnt.h"

extern idx_t _floor_isqrt_u64(idx_t);       /* provided in the build utilities */

seq_t a(idx_t n)
{
    /* Find the diagonal index d such that d*(d+1)/2 ≤ n < (d+1)*(d+2)/2 */
    idx_t lo = 0;
    idx_t hi = n + 1;          /* hi is exclusive upper bound */
    while (hi - lo > 1) {
        idx_t mid = lo + (hi - lo) / 2;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val <= n)
            lo = mid;
        else
            hi = mid;
    }
    idx_t d = lo;

    idx_t start = d * (d + 1) / 2;
    idx_t k = n - start;          /* offset within the diagonal, 0 ≤ k ≤ d */
    idx_t j = d - k;

    unsigned p = parity_popcnt(k) ^ parity_popcnt(j);
    return (seq_t)p;
}
