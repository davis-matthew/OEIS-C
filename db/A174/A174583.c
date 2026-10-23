/*
 * A174583  a(k) is the least n such that the concatenation (n - k)"n is a
 *          prime number, for k >= 0.
 * https://oeis.org/A174583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = least m > n such that the concatenation (m-n)""m is
 *            prime. (Search)
 * Range:     u8 n <= 9, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t k)
{
    /* Search for the least n > k with (n - k)""n prime. */
    for (idx_t n = k + 1; ; ++n) {
        /* Only consider n ending in 1,3,7,9 to avoid obvious composites. */
        uint8_t ld = n % 10;
        if (ld == 1 || ld == 3 || ld == 7 || ld == 9) {
            seq_t concat = concat_parts((idx_t)(n - k), (idx_t)n);
            if (is_prime_big(concat))
                return (seq_t)n;
        }
    }
}
