/*
 * A135517  a(n) = 2^(A091090(n)-1).
 * https://oeis.org/A135517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(A091090(n)-1). Recurrence: e(0) = e(1) = 0, e(n) = 0
 *            if n is even, e(n) = e((n-1)/2) + 1 if n is odd and n > 1. Here
 *            a(n) = 2^e(n). We compute e(n) by repeatedly replacing an odd n
 *            > 1 with (n-1)/2 until an even value is reached. The exponent is
 *            the number of replacements performed. The...
 * Range:     u8 n <= 510, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return pow2(0);      /* a(0) = a(1) = 1 */

    idx_t e = 0;
    /* Count replacements while n is odd and greater than 1 */
    while ((n & 1ULL) && n > 1) {
        e++;
        n = (n - 1) / 2;
    }
    return pow2(e);
}
