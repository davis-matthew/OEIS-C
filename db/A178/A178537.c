/*
 * A178537  a(n) = (2n+1)^(2n+1) mod 2^(2n+1).
 * https://oeis.org/A178537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n+1)^(2n+1) mod 2^(2n+1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* base = 2n + 1 (always odd) */
    seq_t base = (seq_t)n * 2 + 1;

    /* exponent = 2n + 1 */
    idx_t exp = 2 * n + 1;

    /* modulus mask: 2^exp - 1 (keeps the lower exp bits) */
    seq_t mask = pow2(exp) - 1;

    seq_t result = 1;   /* accumulating result */
    seq_t cur    = base;/* current power of base */

    while (exp)
    {
        if (exp & 1)
            result = (result * cur) & mask;   /* multiply and keep lower bits */

        cur = (cur * cur) & mask;             /* square and keep lower bits */
        exp >>= 1;                            /* next bit of exponent */
    }

    return result;   /* (2n+1)^(2n+1) mod 2^(2n+1) */
}
