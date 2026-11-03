/*
 * A133600  Row sums of triangle A133599.
 * https://oeis.org/A133600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n if n is even, a(n) = 7*2^(n-3) if n is odd (n ≥ 1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the trivial cases */
    if (n == 0) {
        return 0;        /* not part of the OEIS offset, but harmless */
    }
    if (n == 1) {
        return 1;
    }

    /* Even index: a(n) = 2^n */
    if ((n & 1) == 0) {
        return pow2(n);
    }

    /* Odd index and n ≥ 3: a(n) = 7 * 2^(n-3) */
    return (seq_t)7 * pow2(n - 3);
}
