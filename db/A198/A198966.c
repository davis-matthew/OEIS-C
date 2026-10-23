/*
 * A198966  a(n) = 8*9^n-1.
 * https://oeis.org/A198966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*9^n-1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;          /* 9^0 */
    seq_t base   = 9;
    idx_t exp    = n;

    while (exp > 0) {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    return result * (seq_t)8 - (seq_t)1;
}
