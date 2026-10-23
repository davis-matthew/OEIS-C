/*
 * A164370  Sequence A005418 written in base 2.
 * https://oeis.org/A164370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less, easy
 * Formula:   a(n) = 10^(n-2) + 10^((n-2)/2) for n ≥ 3, a(1) = 1, a(2) = 10.
 *            (The sequence consists of decimal numbers whose digits are the
 *            binary representation of sums of two powers of two.)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n <= 2)            /* n = 1 → 10^0 = 1, n = 2 → 10^1 = 10 */
        return pow10(n - 1);

    idx_t k = n - 2;       /* position of the most significant 1
                             (exponent of the larger power of two) */
    idx_t j = k / 2;       /* position of the second 1 (floor(k/2)) */

    return pow10(k) + pow10(j);
}
