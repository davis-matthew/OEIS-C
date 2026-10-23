/*
 * A116453  Third smallest number with exactly n prime factors.
 * https://oeis.org/A116453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 5, a(n) = 9 * 2^(n-2) for n > 1 (equivalently, a(n) = 2 *
 *            a(n-1) for n > 2)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n<1, but safe */
    if (n == 1)
        return 5;          /* first term */
    /* n > 1: compute 9 * 2^(n-2) */
    return pow2(n - 2) * (seq_t)9;
}
