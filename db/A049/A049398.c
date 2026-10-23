/*
 * A049398  a(n) = (n+9)!/9!.
 * https://oeis.org/A049398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+9)!/9!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  // for NULL if needed; types seq_t and idx_t are supplied by the build

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t limit = n + 9;                // upper bound of multiplication
    for (idx_t k = 10; k <= limit; ++k) // multiply through 10..(n+9)
        result *= (seq_t)k;
    return result;
}
