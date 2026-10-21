/*
 * A206032  a(n) = Product_{d|n} sigma(d) where sigma = A000203.
 * https://oeis.org/A206032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{d|n} sigma(d) where sigma(d) = sigma_of(d)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 47, u64 n <= 119, u128 n <= 479,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t d = 1; d*d <= n; ++d) {
        if (n % d == 0) {
            prod *= sigma_of(d);
            idx_t other = n / d;
            if (other != d)
                prod *= sigma_of(other);
        }
    }
    return prod;
}
