/*
 * A092914  a(n) = (n-1)*(n-2)*...*(n-r) with the least value of r so that n
 *          divides a(n).
 * https://oeis.org/A092914
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*(n-2)*...*(n-r) where r is the least positive
 *            integer such that n divides the product; a(n) = 0 for prime n
 *            (and for n < 6, as defined in the OEIS entry)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 21, u64 n <= 33, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 6)
        return 0;

    if (is_prime(n))
        return 0;

    seq_t product = 1;
    for (idx_t r = 1; r < n; ++r) {
        product *= (seq_t)(n - r);
        if (product % (seq_t)n == 0)
            return product;
    }

    return 0;
}
