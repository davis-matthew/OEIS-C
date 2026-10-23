/*
 * A103167  a(n) = 2^n mod reverse(2^n).
 * https://oeis.org/A103167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 2^n mod reverse(2^n)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/rev.h"

seq_t a(idx_t n)
{
    seq_t v = pow2(n);      /* 2^n */
    seq_t rv = rev(v);      /* reverse decimal digits of 2^n */
    if (rv == 0)            /* safeguard, though never occurs for powers of 2 */
        return 0;
    return v % rv;          /* remainder of v divided by rv */
}
