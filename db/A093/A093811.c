/*
 * A093811  Sum of the digital products of the divisors of n.
 * https://oeis.org/A093811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Sum_{d|n} digit_product(d)
 * Range:     u8 n <= 286, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of the digital products of all divisors of n. */
#include <stdbool.h>
#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined for n = 0, safe guard */

    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            sum += digit_product((seq_t)d);
            idx_t other = n / d;
            if (other != d)
                sum += digit_product((seq_t)other);
        }
    }
    return sum;
}
