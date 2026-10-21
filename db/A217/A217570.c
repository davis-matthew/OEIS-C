/*
 * A217570  Numbers n such that floor(sqrt(n)) =
 *          floor(n/(floor(sqrt(n))-1))-1.
 * https://oeis.org/A217570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (m^2 + k) where m>=3 is the smallest integer such that
 *            (m-2)(m-1)/2 >= n, k = n - (m-3)(m-2)/2 - 1. This enumerates the
 *            triangle: n=3 : 9 n=4 : 16 17 n=5 : 25 26 27 etc
 * Range:     u8 n <= 91, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t triangular_half(idx_t x)
{
    return (idx_t)((__int128)x * (x + 1) / 2);
}

static inline idx_t find_j(idx_t pos)
{
    idx_t low = 0, high = 1;
    while ((__int128)high * (high + 1) / 2 < pos)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= pos)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t pos)
{
    /* Find j such that j(j+1)/2 >= pos
       where m = j + 2.  */
    idx_t j = find_j(pos);
    idx_t m = j + 2;

    /* cumulative count before the block of m: (j-1)*j/2 */
    idx_t prev = triangular_half(j - 1);
    idx_t offset = pos - prev - 1;          /* k */

    return (seq_t)m * (seq_t)m + (seq_t)offset;
}
