/*
 * A054442  Second convolution of A001405 (central binomial numbers).
 * https://oeis.org/A054442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2*k) = -3*2^(2*k+1) + binomial(2*(k+1), k+1)*(4*k+7)/2
 *            a(2*k+1) = -3*4^(k+1) + binomial(2*(k+2), k+2)*(2*k+5)/2 This
 *            implementation uses the provided binomial() helper and computes
 *            the required powers by repeated multiplication to avoid overflow
 *            on types smaller than 128 bits
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 26, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* even index */
    if ((n & 1ULL) == 0) {
        idx_t k = n >> 1;                         /* k = n/2 */

        /* bin = C(2*(k+1), k+1) */
        seq_t bin = binomial(2 * (k + 1), k + 1);

        /* part = bin * (4*k+7) / 2 */
        seq_t part = bin * (seq_t)(4 * k + 7);
        part /= (seq_t)2;

        /* pow = 2^(2*k+1)  computed by repeated *2 */
        seq_t pow = (seq_t)1;
        for (idx_t i = 0; i < 2 * k + 1; ++i)
            pow *= (seq_t)2;

        /* result = part - 3*pow */
        return part - (seq_t)3 * pow;
    }

    /* odd index */
    {
        idx_t k = (n - 1) >> 1;                   /* k = (n-1)/2 */

        /* bin = C(2*(k+2), k+2) */
        seq_t bin = binomial(2 * (k + 2), k + 2);

        /* part = bin * (2*k+5) / 2 */
        seq_t part = bin * (seq_t)(2 * k + 5);
        part /= (seq_t)2;

        /* pow = 4^(k+1) = 2^(2*(k+1))  computed by repeated *2 */
        seq_t pow = (seq_t)1;
        for (idx_t i = 0; i < 2 * (k + 1); ++i)
            pow *= (seq_t)2;

        /* result = part - 3*pow */
        return part - (seq_t)3 * pow;
    }
}
