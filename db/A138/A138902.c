/*
 * A138902  a(n) = d!, where d is the number of digits in n.
 * https://oeis.org/A138902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = (d)! where d is the number of decimal digits of n (with d
 *            = 1 for n = 0). The factorial is computed by repeated
 *            multiplication to avoid large constant literals
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    /* Get the number of decimal digits of n.
       The provided digit_count routine returns 1 for n == 0. */
    unsigned int d = (unsigned int)digit_count((uint64_t)n);

    /* Compute factorial(d) in type seq_t.  For d <= 20 this is safe
       for all supported seq_t sizes; for smaller seq_t types the
       result is only needed when it fits in that type. */
    seq_t result = 1;
    for (unsigned int i = 2; i <= d; ++i)
        result *= i;

    return result;
}
