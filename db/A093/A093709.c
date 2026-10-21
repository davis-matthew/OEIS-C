/*
 * A093709  Characteristic function of squares or twice squares.
 * https://oeis.org/A093709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult
 * Formula:   A093709: a(n) = 1 if n is a square or twice a square, else 0. *
 *            The sequence is multiplicative with a(0)=1, all odd prime
 *            exponents even * and the exponent of 2 arbitrary. For n>0 this
 *            is equivalent to the * odd part of n being a perfect square
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_uint64(idx_t x)
{
    /* integer square root via binary search, using 128‑bit arithmetic to avoid overflow */
    idx_t lo = 0, hi = x;
    idx_t ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= x) {
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
    if (n == 0) return 1;          /* a(0) = 1 by definition */
    /* Remove all factors of 2 to get the odd part */
    idx_t odd = n;
    while ((odd & 1ULL) == 0) odd >>= 1;
    /* Check if the odd part is a perfect square */
    idx_t r = isqrt_uint64(odd);
    __uint128_t r_sq = (__uint128_t)r * r;
    if (r_sq == odd) return 1;
    return 0;
}
