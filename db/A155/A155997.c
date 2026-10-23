/*
 * A155997  Triangle read by rows: T(n, k) = f(n, k) + f(n, n-k), where f(n,
 *          k) = binomial(n, k)*(1 + (-1)^k)/2.
 * https://oeis.org/A155997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where T(r, k) = binomial(r, k) * (2 + (-1)^k * (1
 *            + (-1)^r)) / 2, and the index n is converted to (r, k) by moving
 *            through the rows of the triangle (offset 0)
 * Range:     u8 n <= 39, u16 n <= 178, u32 n <= 610, u64 n <= 2373,
 *            u128 n <= 5150, big any n
 * Helpers:   binom_nat
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_nat.h"

/* Integer square‑root of a 64‑bit unsigned value (floor) */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = (idx_t)1 << 32, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= v) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Find the row r that contains index n:
       r satisfies r*(r+1)/2 <= n < (r+1)*(r+2)/2.  */
    idx_t r = (isqrt_u64((idx_t)(((__uint128_t)n * 8u) + 1u)) - 1u) >> 1;
    idx_t base = (idx_t)(((__uint128_t)r * (r + 1u)) >> 1);  /* first index of row r */
    idx_t k = n - base;                                     /* column index within the row */

    seq_t bin = binom_nat(r, k);

    /* Compute T(r, k) according to the formula.
       When r is odd, T = bin.
       When r is even:
          if k is odd → T = 0
          if k is even → T = 2 * bin */
    if ((r & 1u) == 0u) {       /* r even */
        if ((k & 1u) == 0u)     /* k even */
            return bin * (seq_t)2;
        else                    /* k odd */
            return (seq_t)0;
    } else {                    /* r odd */
        return bin;
    }
}
