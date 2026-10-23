/*
 * A165957  Product of the digits of the n-th nonprime.
 * https://oeis.org/A165957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = product of digits of the n‑th nonprime number
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 0;

    /* Find the n-th nonprime */
    for (m = 1; ; ++m) {
        if (!is_prime(m)) {
            ++count;
            if (count == n) {
                break;
            }
        }
    }

    /* Compute product of decimal digits of m */
    seq_t prod = 1;
    idx_t tmp = m;
    while (tmp > 0) {
        uint64_t digit = tmp % 10;
        prod *= (seq_t)digit;
        tmp /= 10;
    }

    return prod;
}
