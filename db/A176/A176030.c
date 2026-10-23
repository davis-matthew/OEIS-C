/*
 * A176030  a(n) = n^(sigma_2(n)).
 * https://oeis.org/A176030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^(sigma_2(n)) where sigma_2(n) is the sum of squares of
 *            divisors
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* Out of range; not used for the odd OEIS offset */
    /* Compute sigma_2(n) = sum_{d|n} d^2 */
    seq_t sigma2 = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            seq_t d2 = (seq_t)d * (seq_t)d;
            sigma2 += d2;
            idx_t e = n / d;
            if (e != d) {
                seq_t e2 = (seq_t)e * (seq_t)e;
                sigma2 += e2;
            }
        }
    }
    /* Compute n^sigma2 */
    seq_t base = (seq_t)n;
    return big_pow(base, sigma2);
}
