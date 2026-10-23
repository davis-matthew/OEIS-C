/*
 * A106189  Expansion of 1/((1-2x^2)sqrt(1-4x)).
 * https://oeis.org/A106189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..floor(n/2)} binomial(2(n-2k), n-2k) * 2^k
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Binomial coefficients and powers of two are provided by the
   utils library, so we include the appropriate headers. */
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 2;          /* floor(n/2) */
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t m = n - 2 * k;                    /* m = n - 2k */
        seq_t bin = binomial(2 * m, m);         /* binomial(2m, m) */
        seq_t term = bin * pow2(k);             /* bin * 2^k */
        sum += term;
    }
    return sum;
}
