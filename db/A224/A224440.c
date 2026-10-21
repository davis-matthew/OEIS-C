/*
 * A224440  a(n) = sigma(n)^(n-1).
 * https://oeis.org/A224440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n)^(n-1) (sigma = sum of divisors)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) /* offset 1, but guard against illegal input */
        return 0;

    seq_t sigma = 0;
    /* compute sum of divisors of n */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            sigma += (seq_t)d;
            idx_t other = n / d;
            if (other != d)
                sigma += (seq_t)other;
        }
    }

    /* exponent is n-1, cast to seq_t safely */
    return big_pow(sigma, (seq_t)(n - 1));
}
