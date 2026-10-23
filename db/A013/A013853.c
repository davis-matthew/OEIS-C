/*
 * A013853  a(n) = 9^(5*n + 4).
 * https://oeis.org/A013853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^(5*n + 4)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute exponent e = 5*n + 4 */
    idx_t e = 5 * n + (idx_t)4;

    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)9;

    /* Fast exponentiation by squaring */
    while (e) {
        if (e & (idx_t)1)
            result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}
