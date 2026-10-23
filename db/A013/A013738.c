/*
 * A013738  a(n) = 6^(3*n + 1).
 * https://oeis.org/A013738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^(3*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 16,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* exponent = 3*n + 1 */
    idx_t exp = 3 * n + 1;

    seq_t base   = 6;
    seq_t result = 1;

    /* binary exponentiation */
    while (exp > 0) {
        if (exp & 1u)
            result = seq_mul(result, base);
        base = seq_mul(base, base);
        exp >>= 1;
    }

    return result;
}
