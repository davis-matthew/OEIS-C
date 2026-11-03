/*
 * A066090  a(n) = binomial(sigma(n), n).
 * https://oeis.org/A066090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = binomial(sigma(n),n)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 17, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 197 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Offset is 1, but guard against 0 */
    seq_t sigma = sigma_of(n);
    return binomial((idx_t)sigma, n);
}
