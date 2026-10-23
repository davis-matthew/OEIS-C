/*
 * A036565  Triangle of numbers in which i-th row is {2^(i-j)*7^j, 0<=j<=i}; i
 *          >= 0.
 * https://oeis.org/A036565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 2^(i-j) * 7^j, where i is the largest integer with
 *            i*(i+1)/2 <= n, and j = n - i*(i+1)/2 (0 <= j <= i)
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Find the row index i so that i*(i+1)/2 <= n. */
    idx_t lo = 0;
    idx_t hi = n + 1;              /* hi is exclusive; guarantees upper bound */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        /* Use a 128‑bit intermediate to avoid overflow */
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n)
            lo = mid;
        else
            hi = mid;
    }
    idx_t i = lo;

    /* Cumulative count of elements before row i. */
    idx_t accum = (idx_t)((__int128)i * (i + 1) / 2);

    /* Position within the row. */
    idx_t j = n - accum;

    /* Compute the term as 2^(i-j) * 7^j. */
    seq_t part1 = pow2(i - j);          /* 2^(i-j)   */
    seq_t part2 = pow_uint(7, j);       /* 7^j       */
    return part1 * part2;
}
