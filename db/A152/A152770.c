/*
 * A152770  Sum of proper divisors minus the number of proper divisors of n:
 *          a(n) = sigma(n) - n - d(n) + 1.
 * https://oeis.org/A152770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n) - n - d(n) + 1 where sigma(n) is the sum of all
 *            positive divisors of n and d(n) is the number of positive
 *            divisors of n. a(n) is the sum of proper divisors of n minus the
 *            number of proper divisors of n plus 1
 * Range:     u8 n <= 167, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sigma_minus_n_and_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_minus_n_and_divisors.h"

seq_t a(idx_t n)
{
    if (n < 1) {
        /* Undefined for n < 1; return 0 for safety. */
        return 0;
    }
    seq_t d;
    seq_t sigma_n = sigma_minus_n_and_divisors(n, &d);
    /* proper sum = sigma(n) - n */
    seq_t proper_sum = sigma_n - (seq_t)n;
    /* a(n) = proper_sum - d + 1 */
    seq_t result = proper_sum + 1 - d;
    return result;
}
