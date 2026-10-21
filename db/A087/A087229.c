/*
 * A087229  Exponent of p=2 in 12n+4 = 3(4n+1)+1.
 * https://oeis.org/A087229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 + v2(3*n + 1) (exponent of 2 in 12n+4)
 * Range:     u8 n <= 84, u16 n <= 9541, u32 n <= 9541, u64 n <= 9541,
 *            u128 n <= 9541, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute 3*n + 1 */
    seq_t x = (seq_t)3 * (seq_t)n + 1;

    /* Count trailing factors of 2 (legendre symbol v2). */
    seq_t v = 0;
    while ((x & 1) == 0) {
        x >>= 1;
        ++v;
    }

    /* a(n) = 2 + v2(3*n + 1) */
    return v + 2;
}
