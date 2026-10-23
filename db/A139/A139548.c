/*
 * A139548  Triangle T(n,k) with the coefficient of [x^k] of the polynomial
 *          (2*(x+1)^2)^n in row n, column k, 0<=k<=2n.
 * https://oeis.org/A139548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 2^⌊√n⌋ * C(2⌊√n⌋, n - ⌊√n⌋^2)
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 152, u64 n <= 546,
 *            u128 n <= 1976, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

/* Integer floor square root for unsigned 64‑bit indices */
static idx_t floor_sqrt_u64(idx_t n)
{
    if (n == 0u) return 0u;
    idx_t low = 0u;
    idx_t high = (idx_t)1u << 32;          /* > sqrt(max u64) */
    if (high > n) high = n + 1u;
    while (low < high) {
        idx_t mid = (low + high) >> 1u;
        if (mid <= n / mid)
            low = mid + 1u;
        else
            high = mid;
    }
    return low - 1u;
}

seq_t a(idx_t n)
{
    idx_t r = floor_sqrt_u64(n);          /* r = ⌊√n⌋            */
    idx_t k = n - r * r;                  /* column offset      */
    return pow2(r) * binomial(2 * r, k);  /* 2^r * C(2r, k)     */
}
