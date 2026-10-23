/*
 * A136688  Triangle of polynomials F(x,n) = x*F(x,n-1) + 2*F(x,n-2).
 * https://oeis.org/A136688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the coefficient of x^k in the polynomial F(x,r), where r
 *            is the unique row with T(r‑1) < n ≤ T(r) (T(m)=m(m+1)/2) and k =
 *            n – T(r‑1) – 1. The coefficient equals binom(r – j – 1, j) * 2^j
 *            with j = (r‑1 – k)/2 when r‑1 and k have the same parity,
 *            otherwise 0
 * Range:     u8 n <= 59, u16 n <= 195, u32 n <= 641, u64 n <= 2364,
 *            u128 n <= 5050, big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>          /* sqrt */
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1, but guard anyway */
        return (seq_t)0;

    /* Find the row r such that r*(r+1)/2 >= n > (r-1)*r/2.  */
    idx_t r = (idx_t)((sqrt((double)(8.0 * n + 1.0)) - 1.0) / 2.0);
    while (((__uint128_t)r * (r + 1)) / 2 < n) r++;
    while (r > 0 && (((__uint128_t)(r - 1) * r) / 2) >= n) r--;

    /* Column index (0‑based) within the row */
    idx_t first = ((r - 1) * r) / 2 + 1;
    idx_t k = n - first;                /* 0 ≤ k < r */

    /* Parity check – coefficient is zero if r‑1 and k differ in parity. */
    if (((r - 1) & 1) != (k & 1))
        return (seq_t)0;

    /* j = (r‑1 – k)/2.  */
    idx_t j = ((r - 1) - k) >> 1;

    /* Compute binomial coefficient and multiply by 2^j. */
    seq_t coeff = binom(r - j - 1, j);
    coeff = coeff * pow2(j);
    return coeff;
}
