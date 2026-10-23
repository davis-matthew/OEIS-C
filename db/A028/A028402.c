/*
 * A028402  Number of types of Boolean functions of n variables under a
 *          certain group.
 * https://oeis.org/A028402
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*(2^(n-1)+1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Returns the a(n) value for the OEIS sequence A028402.
   The function is defined for all n >= 2; for n < 2 it returns 0
   (the behavior for indices below the sequence offset is undefined). */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;          /* undefined part of the sequence */

    seq_t half = pow2(n - 1);      /* 2^(n-1) */
    return half * (half + 1);
}
