/*
 * A127510  Triangle T(n,k) = k*mobius(n).
 * https://oeis.org/A127510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, easy, sign
 * Formula:   a(n) = (n - T_{t-1}) * mobius(t), where t is the smallest
 *            integer such that t(t+1)/2 ≥ n and T_m = m(m+1)/2
 * Range:     i8 n <= 55, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <inttypes.h>
#include "../utils/mobius.h"

seq_t a(idx_t m) {
    if (m == 0) return 0;          /* offset starts at 1, but guard against 0 */
    idx_t low = 1, high = UINT64_MAX;
    /* Binary search for the row index t */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= m)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t t = low;
    unsigned __int128 prev_tri = (unsigned __int128)(t - 1) * t / 2;
    idx_t k = m - (idx_t)prev_tri;  /* column index in the triangle */
    seq_t mu = mobius(t);
    return (seq_t)k * mu;
}
