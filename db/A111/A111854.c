/*
 * A111854  Number of numbers m <= n such that 4 equals the first digit after
 *          decimal point of square root of n in decimal representation.
 * https://oeis.org/A111854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{ 1 ≤ m ≤ n : the first digit after the decimal point of
 *            √m is 4 }
 * Range:     u8 n <= 2440, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u128.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t i = 1; i <= n; ++i) {
        unsigned __int128 val = (unsigned __int128)i * 100ULL; /* floor((10√i)²) */
        uint64_t s10 = isqrt_u128(val);                   /* floor(10√i)   */
        if ((s10 % 10) == 4) {
            count++;
        }
    }
    return count;
}
