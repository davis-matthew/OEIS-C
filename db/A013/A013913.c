/*
 * A013913  a(n) = 24^(5*n + 4).
 * https://oeis.org/A013913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24^(5*n + 4)
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 1, u128 n <= 4,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Base 24 as seq_t */
    const seq_t base = 24;
    /* Exponent 5*n + 4 */
    const idx_t exp = 5 * n + 4;
    /* Compute the power using the utility function */
    return seq_pow(base, exp);
}
