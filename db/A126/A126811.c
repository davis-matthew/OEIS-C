/*
 * A126811  Duplicate of A098108.
 * https://oeis.org/A126811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 1 if n is a perfect square of an odd integer, otherwise 0
 * Range:     i8 n <= 105, i16 n <= 105, i32 n <= 105, i64 n <= 105,
 *            i128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a uint64_t using binary search.
   The product mid*mid is computed in unsigned __int128 to avoid overflow. */
static inline idx_t _isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 1)              /* sequence is defined for n ≥ 1, return 0 otherwise */
        return 0;

    idx_t r = _isqrt_u64(n);
    if (r * r == n && (r & 1))   /* r is odd */
        return 1;
    return 0;
}
