/*
 * A199397  Binary XOR of 3^k as k varies from 0 to n.
 * https://oeis.org/A199397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = XOR_{k=0}^{n} 3^k
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t result = 0;          /* accumulator for XOR */
    seq_t term   = 1;          /* current power 3^k */

    for (idx_t k = 0; k <= n; ++k)
    {
        result ^= term;        /* include current power */

        /* prepare next power only if we have not reached the last one */
        if (k != n)
            term = mul_small(term, 3); /* term *= 3 */
    }

    return result;
}
