/*
 * A013875  a(n) = 15^(5*n + 2).
 * https://oeis.org/A013875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15^(5*n + 2)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 6,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* compute exponent 5*n + 2 */
    idx_t exp = n * 5 + 2;

    /* exponentiation by squaring, using seq_mul for arbitrary precision */
    seq_t result = 1;
    seq_t base = 15;           /* 15 as seq_t */

    while (exp) {
        if (exp & 1)
            result = seq_mul(result, base);
        base = seq_mul(base, base);
        exp >>= 1;
    }

    return result;
}
