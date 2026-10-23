/*
 * A190997  Product of digits of all the divisors of n.
 * https://oeis.org/A190997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = product of the decimal digits of the concatenation of all
 *            divisors of n. If n is a multiple of 10, a(n) = 0 (since all its
 *            divisors contain the digit 0)
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 143, u64 n <= 575,
 *            u128 n <= 3863, big any n
 * Helpers:   _digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_digit_product.h"

seq_t a(idx_t n)
{
    /* Multiples of 10 give 0 because some divisor (namely 10 itself) has digit 0. */
    if (n % 10 == 0) {
        return 0;
    }

    seq_t result = 1;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0) {
            continue;
        }
        /* process divisor d */
        result *= _digit_product(d);

        /* process the paired divisor n/d if distinct */
        idx_t e = n / d;
        if (e != d) {
            result *= _digit_product(e);
        }
    }
    return result;
}
