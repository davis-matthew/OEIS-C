/*
 * A115025  a(n) = n-th element of n-th row of triangle shown below.
 * https://oeis.org/A115025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + bitreverse(n-1, n-1) (reverse the lower n-1 bits of
 *            n-1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* For n = 1 the width is zero; the reverse of 0 is 0, so a(1)=1. */
    if (n == 0)
        return 0;   /* offset is 1, this case should not occur */

    const idx_t w = n - 1;          /* number of bits to reverse */
    seq_t rev = 0;
    seq_t x   = (seq_t)(n - 1);

    for (idx_t i = 0; i < w; ++i) {
        rev = (rev << 1) | ((x >> i) & 1);
    }

    return rev + 1;   /* 1‑based value */
}
