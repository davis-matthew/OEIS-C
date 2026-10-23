/*
 * A017100  a(n) = (8*n + 2)^12.
 * https://oeis.org/A017100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 2)^12
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 4, u128 n <= 202,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the base: 8*n + 2, cast to seq_t to avoid overflow issues
       when idx_t is larger than seq_t. */
    seq_t base = ((seq_t)8 * n) + (seq_t)2;

    /* Exponentiate a fixed power: use exponentiation by squaring (12 is fixed). */
    seq_t result = 1;
    seq_t power = base;
    unsigned int e = 12;

    while (e) {
        if (e & 1u)
            result *= power;
        power *= power;
        e >>= 1u;
    }

    return result;
}
