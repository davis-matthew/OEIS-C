/*
 * A109975  Second differences of A045623, prefixed by an initial 1.
 * https://oeis.org/A109975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-3) * (n+6) for n > 1, a(0) = 1, a(1) = 2, a(2) = 4
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 2;
    if (n == 2)
        return 4;
    /* For n >= 3: compute (n+6) * 2^(n-3) */
    seq_t val = (seq_t)(n + 6);
    val *= pow2(n - 3);
    return val;
}
