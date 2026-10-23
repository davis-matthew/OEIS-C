/*
 * A130321  Triangle, (2^0, 2^1, 2^2, ...) in every column.
 * https://oeis.org/A130321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2^m, where m = (t*t + 3*t + 4)/2 - n, t = floor((-1 +
 *            sqrt(8*n - 7))/2)
 * Range:     u8 n <= 35, u16 n <= 135, u32 n <= 527, u64 n <= 2079,
 *            u128 n <= 7380, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard integer types */
#include <stdint.h>
/* Utility for computing powers of two */
#include "../utils/pow2.h"

/* Binary search for the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static idx_t
find_row(idx_t n)
{
    idx_t low  = 0;
    idx_t high = (idx_t)1ULL << 33;  /* upper bound: > sqrt(2*2^64) ≈ 6.3e9 */

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __uint128_t prod = (__uint128_t)mid * (mid + 1) / 2;
        if (prod <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t
a(idx_t n)
{
    idx_t r = find_row(n);                     /* row index */
    __uint128_t cum128 = (__uint128_t)r * (r + 1) / 2;
    idx_t cum = (idx_t)cum128;                 /* number of entries before row r */
    idx_t c   = n - cum;                       /* column index within the row */
    idx_t exp = r - c;                         /* exponent of 2 for this entry */
    return pow2(exp);                          /* 2^exp */
}
