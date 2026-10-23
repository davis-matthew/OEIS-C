/*
 * A140062  a(n) = 101*2^(n-1) - 100.
 * https://oeis.org/A140062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 101*2^(n-1) - 100 (offset = 1)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef __a__h__
#define __a__h__

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n == 0 we simply
       return 0, although this value is outside the OEIS offset
       and is not used for validation. */
    if (n == 0)
        return 0;

    /* Compute 2^(n-1) by repeated doubling. This avoids
       undefined behaviour for left‑shift on unsigned types. */
    seq_t two_pow = 1;
    for (idx_t i = 1; i < n; ++i) {
        two_pow *= 2;          // multiply by 2
    }

    /* a(n) = 101 * 2^(n-1) - 100 */
    two_pow *= 101;
    two_pow -= 100;

    return two_pow;
}

#endif /* __a__h__ */
