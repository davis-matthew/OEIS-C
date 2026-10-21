/*
 * A075554  Denominators in the Maclaurin series for arctan(1+x).
 * https://oeis.org/A075554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 1 if n mod 4 == 0 a(n) = n * 2^{floor((n-1)/2)+1}
 *            otherwise
 * Range:     u8 n <= 8, u16 n <= 22, u32 n <= 52, u64 n <= 114, u128 n <= 240,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n >= 1 */
    if ((n & 3ULL) == 0)          /* multiples of 4 */
        return (seq_t)1;

    /* exponent = floor((n-1)/2) + 1 */
    idx_t exp = ((n - 1) >> 1) + 1;

    seq_t result = (seq_t)n;
    for (idx_t i = 0; i < exp; ++i)
        result *= 2;

    return result;
}
