/*
 * A130329  A059268 * A130321.
 * https://oeis.org/A130329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   triangle entry a(r,c) for row r, column c (0‑based) a(r,c) = 2^c
 *            * (4^{(r-c+1)} - 1) / 3. The sequence a(n) is the triangle read
 *            row‑wise (a(0)=1, a(1)=5, a(2)=2, …)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"   /* pow2(idx_t exp) -> seq_t   */

/* integer square‑root of a non‑negative 64‑bit integer.
   Returns floor(sqrt(x)).  No floating‑point operations are used. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = (x >> 1) + 1;  /* high is always >= sqrt(x) */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;  /* upper midpoint */
        if (mid > 0 && mid > x / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/* Return nth term of OEIS A130329. */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t t = n * 8 + 1;              /* 8*n + 1 for quadratic solving */
    idx_t s = isqrt_u64(t);           /* floor(sqrt(8*n+1)) */
    idx_t r = (s - 1) >> 1;           /* r = floor((sqrt(8*n+1)-1)/2) */

    idx_t tri = r * (r + 1) / 2;      /* triangular number tri(r) = r(r+1)/2 */
    idx_t c = n - tri;                /* column index */

    /* Compute the formula: 2^c * (4^{(r-c+1)} - 1) / 3. */
    seq_t pow2_c = pow2(c);                     /* 2^c                  */
    seq_t pow2_2k = pow2(2 * (r - c + 1));      /* 4^{(r-c+1)} = 2^{2*(r-c+1)} */
    seq_t temp = pow2_2k - 1;                   /* 4^{k} - 1            */
    temp /= 3;                                  /* divide by 3 (exact)   */
    return pow2_c * temp;                       /* final result         */
}
