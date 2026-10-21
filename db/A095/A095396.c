/*
 * A095396  Modified juggler map: for even numbers: a(n) = floor(n^(2/3)) and
 *          for odd n: a(n) = floor(n^(3/2)) = floor(sqrt(n^3)).
 * https://oeis.org/A095396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^(2/3)) for even n, floor(n^(3/2)) for odd n
 * Range:     u8 n <= 40, u16 n <= 1626, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   icbrt_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/icbrt_idx.h"

/* Integer square root: floor(sqrt(x)) for non‑negative x */
static inline idx_t isqrt_idx(idx_t x)
{
    if (x == 0) return 0;           /* avoid division by zero in the search */
    idx_t left = 1, right = x, ans = 0;
    while (left <= right) {
        idx_t mid = (left + right) >> 1;
        if (mid <= x / mid) {        /* mid*mid <= x  */
            ans = mid;
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term.  Return 0 for n == 0. */
    if (n == 0) return (seq_t)0;

    if ((n & 1ULL) == 0) {            /* even n: floor(n^(2/3)) = floor(cuberoot(n²)) */
        idx_t n_sq = n * n;            /* n² fits in 64‑bit for n up to ~10⁵ */
        idx_t r   = icbrt_idx(n_sq);    /* integer cube root */
        return (seq_t)r;
    } else {                           /* odd n: floor(n^(3/2)) = floor(sqrt(n³))      */
        idx_t n_cu = n * n * n;        /* n³ fits in 64‑bit for n up to ~10⁵ */
        idx_t r   = isqrt_idx(n_cu);    /* integer square root */
        return (seq_t)r;
    }
}
