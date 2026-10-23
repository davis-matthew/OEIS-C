/*
 * A010794  a(n) = n!*(n+5)!/5!.
 * https://oeis.org/A010794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*(n+5)!/5!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t mult = (seq_t)i * (seq_t)(i + 5);
        result *= mult;
    }
    return result;
}
