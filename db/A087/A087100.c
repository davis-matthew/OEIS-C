/*
 * A087100  A000225 (2^n - 1) interlaced with A008593 (11n).
 * https://oeis.org/A087100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 11*(n/2) for n even; a(n) = 2^((n+1)/2) - 1 for n odd
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n & 1)      /* odd index */
    {
        idx_t exp = (n >> 1) + 1;          /* (n+1)/2 */
        return pow2(exp) - 1;
    }
    else           /* even index */
    {
        seq_t val = (seq_t)(n >> 1);        /* n/2 as seq_t */
        return val * 11;
    }
}
