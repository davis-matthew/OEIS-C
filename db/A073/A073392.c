/*
 * A073392  Sixth column of the convolution triangle A073387(n+5, 5), for n >=
 *          0.
 * https://oeis.org/A073392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n-k+5,5) * binomial(n-k,k)
 *            * 2^(n-k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 32, u128 n <= 72,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) from the OEIS definition. */
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t term = binomial(n - k + 5, 5);   /* C(n-k+5, 5)   */
        term *= binomial(n - k, k);           /* C(n-k, k)     */
        term *= pow2(n - k);                  /* 2^(n-k)       */
        sum += term;
    }
    return sum;
}
