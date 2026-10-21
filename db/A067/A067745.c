/*
 * A067745  Numerator of ((3*n - 2)/(n^(2*n - 1)*(2*n - 1)*4^(n - 1))).
 * https://oeis.org/A067745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = odd part of (3*n - 2)
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, a(n) = (3*n - 2) / 2^v2(3*n - 2)  where v2 is the exponent of 2. */

#include <stdbool.h>
#include "../utils/odd_part.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined below offset, harmless fallback */

    seq_t val = (seq_t)n * 3 - 2;   /* compute 3*n - 2 in seq_t */
    return odd_part(val);
}
