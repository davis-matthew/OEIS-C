/*
 * A014221  a(n+1) = 2^a(n) with a(-1) = 0.
 * https://oeis.org/A014221
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n+1) = 2^a(n), a(-1) = 0
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset; the sequence is only defined
       for n >= -1. */
    if (n < -1)
        return (seq_t)0;

    seq_t val = (seq_t)0;   /* a(-1) */

    if (n >= 0) {
        /* Compute the sequence iteratively.
           For i == 0 we set a(0) = 1.
           For i > 0 we set a(i) = 2^a(i-1), using pow2 which expects an
           idx_t exponent.  The exponent fits into idx_t for all n that
           produce a value representable in seq_t (for the tested range). */
        for (idx_t i = 0; i <= n; ++i) {
            if (i == 0)
                val = (seq_t)1;
            else
                val = pow2((idx_t)val);
        }
    }

    return val;
}
