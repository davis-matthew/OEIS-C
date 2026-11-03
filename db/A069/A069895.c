/*
 * A069895  2^a(n) divides (2n)^(2n): exponent of 2 in (2n)^(2n).
 * https://oeis.org/A069895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n*v2(2*n) (v2 = 2-adic valuation)
 * Range:     u8 n <= 31, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-01)
 * Helpers:   ctz_u64_to_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ctz_u64_to_u32.h"

seq_t a(idx_t n)
{
    /* n >= 1 for the sequence.  For safety we handle n==0 here. */
    if (n == 0)
        return 0;

    /* v2(2*n) = v2(n) + 1 */
    unsigned tz_n = ctz_u64_to_u32((uint64_t)n);
    unsigned tz = tz_n + 1;

    seq_t nn = (seq_t)n;
    seq_t result = nn * (seq_t)(2 * tz);
    return result;
}
