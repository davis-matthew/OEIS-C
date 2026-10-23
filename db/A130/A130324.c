/*
 * A130324  A059268^2.
 * https://oeis.org/A130324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(2*j) * (2^(i-j+1)-1) where i = the unique integer with
 *            i*(i+1)/2 ≤ n < (i+1)*(i+2)/2 j = n - i*(i+1)/2 (row‑column
 *            indices in the triangle)
 * Range:     u8 n <= 13, u16 n <= 43, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find i such that i*(i+1)/2 ≤ n < (i+1)*(i+2)/2 */
    idx_t low  = 0;
    idx_t high = 1;

    /* Exponential search to find an upper bound for i */
    while ((unsigned long long)high * (high + 1) / 2 <= n) {
        if (high == 0) break;          /* overflow guard */
        high <<= 1;
    }

    /* Binary search between low and high (high > low) */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned long long t = (unsigned long long)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid + 1;
        else
            high = mid;
    }

    idx_t i   = low - 1;                      /* row index */
    idx_t tri = i * (i + 1) / 2;              /* triangular number T(i) */
    idx_t j   = n - tri;                      /* column index */

    seq_t p1 = pow2(2 * j);                   /* 2^(2*j)   */
    seq_t p2 = pow2(i - j + 1);               /* 2^(i-j+1) */
    seq_t res = p1 * (p2 - 1);                /* final value */

    return res;
}
