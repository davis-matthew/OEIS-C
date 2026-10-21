/*
 * A073394  Eighth column of the convolution triangle A073387(n+7, 7), for n
 *          >= 0.
 * https://oeis.org/A073394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n‑k+7, 7) * binomial(n‑k, k) *
 *            2^(n‑k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 29, u128 n <= 68,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the provided binomial helper and standard integer
   arithmetic. */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t term = binomial(n - k + 7, 7);   /* C(n-k+7, 7)          */
        term *= binomial(n - k, k);           /* C(n-k, k)              */
        term <<= (n - k);                     /* * 2^(n-k)               */
        sum += term;
    }
    return sum;
}
