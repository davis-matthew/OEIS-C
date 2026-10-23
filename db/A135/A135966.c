/*
 * A135966  Triangle, read by rows, where T(n,k) = fibonacci(k(n-k) + 1) for
 *          n>=k>=0.
 * https://oeis.org/A135966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = fib(k * (r - k) + 1) where r = upper row index such that
 *            r*(r+1)/2 <= n < (r+1)*(r+2)/2 and k = n - r*(r+1)/2 (column
 *            index within the row)
 * Range:     u8 n <= 38, u16 n <= 58, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

static inline idx_t _row_of(idx_t n)
{
    /* binary search for the largest r such that r*(r+1)/2 <= n */
    idx_t low = 0;
    idx_t high = 1ULL << 36;          /* sufficiently large upper bound */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __uint128_t pref = (__uint128_t)mid * (mid + 1) / 2;
        if (pref <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = _row_of(n);
    idx_t pref = (idx_t)(((__uint128_t)r * (r + 1)) / 2);
    idx_t k = n - pref;

    __uint128_t t128 = (__uint128_t)k * (r - k) + 1;
    idx_t t = (idx_t)t128;

    return fib(t);
}
