/*
 * A239683  Number of digits in decimal expansion of n^5.
 * https://oeis.org/A239683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of decimal digits of n^5
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   u128_digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/u128_digit_len.h"

seq_t a(idx_t n)
{
    unsigned __int128 r = (unsigned __int128)n;
    r *= r;                          /* n^2 */
    r *= (unsigned __int128)n;        /* n^3 */
    r *= (unsigned __int128)n;        /* n^4 */
    r *= (unsigned __int128)n;        /* n^5 */

    unsigned long long len = u128_digit_len(r);
    return (seq_t)len;
}
