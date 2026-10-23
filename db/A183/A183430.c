/*
 * A183430  One third the number of n X 2 0..3 arrays with no element equal to
 *          its row sum plus its column sum mod 4.
 * https://oeis.org/A183430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9^n / 3 + (n mod 2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* pow9 = 9^n */
    seq_t pow9 = 1;
    idx_t i;
    for (i = 0; i < n; ++i)
        pow9 *= 9;

    /* a(n) = pow9 / 3 (+1 if n is odd) */
    seq_t result = pow9 / 3;
    if (n & 1)
        result += 1;
    return result;
}
