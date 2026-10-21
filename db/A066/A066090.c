/*
 * A066090  a(n) = binomial(sigma(n), n).
 * https://oeis.org/A066090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = binomial(sigma(n),n)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   binomial, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Offset is 1, but guard against 0 */
    seq_t sigma = sigma_of(n);
    return binomial((idx_t)sigma, n);
}
