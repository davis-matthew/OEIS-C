/*
 * A134483  Triangle read by rows: T(n,k) = 2n + k - 2; 1 <= k <= n.
 * https://oeis.org/A134483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*r + (n - (r-1)*r/2) - 2 , * where r = ⌊(√(8*n-7)+1)/2⌋
 *            * This implements the flattening of the triangle * T(n,k) = 2*n
 *            + k - 2 (for 1≤k≤n)
 * Range:     u8 n <= 3740, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root of a 64‑bit unsigned integer.
 * Returns ⌊√x⌋. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = ((idx_t)1 << 32) - 1;   /* hi^2 fits in 64 bits */
    while (lo < hi)
    {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((__uint128_t)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* compute r = floor((sqrt(8*n-7)+1)/2) */
    idx_t rad = 8*n - 7;
    idx_t r = (isqrt_u64(rad) + 1) >> 1;   /* divide by 2 */

    /* offset = (r-1)*r/2 : number of terms before row r */
    idx_t offset = ((r - 1) * r) >> 1;

    /* position inside row: k = n - offset  (1 ≤ k ≤ r) */
    idx_t k = n - offset;

    /* final value: 2*r + k - 2 */
    return (seq_t)(2 * (seq_t)r + (seq_t)k - 2);
}
