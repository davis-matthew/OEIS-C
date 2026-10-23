/*
 * A156203  a(n) = 2*a(n-1) + 3 for n>1, a(1)=14.
 * https://oeis.org/A156203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*2^(n-1) - 3, a(1)=14
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1; values for n < 1 are undefined – return 0 safely. */
    if (n == 0u) {
        return (seq_t)0;
    }
    /* Compute 2^(n-1) using the supplied utility. */
    seq_t v = pow2(n - 1u);
    /* Scale by 17 and subtract 3. */
    v = v * (seq_t)17u;
    v = v - (seq_t)3u;
    return v;
}
