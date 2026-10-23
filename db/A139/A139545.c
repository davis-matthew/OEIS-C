/*
 * A139545  Binomial transform of [1, 0, 0, 4, 0, 0, 7, 0, 0, 10, ...].
 * https://oeis.org/A139545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊(n-1)/3⌋} (3k+1) * binomial(n-1, 3k) (OEIS
 *            A139545)
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of offset; safe guard */
    idx_t m = n - 1;                  /* shift index because formula uses n-1 */
    idx_t maxK = m / 3;
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxK; ++k)
    {
        seq_t comb = binomial(m, k * 3);                /* C(m,3k) */
        seq_t term = comb * (seq_t)(3 * k + 1);         /* (3k+1)*C(m,3k) */
        sum += term;
    }
    return sum;
}
