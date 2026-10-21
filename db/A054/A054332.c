/*
 * A054332  One half of tenth unsigned column of Lanczos triangle A053125
 *          (decreasing powers).
 * https://oeis.org/A054332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(2*n-1) * binomial(2*n+10, 9)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 17, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)5;          /* 2^(-1) * C(10,9) = 5 */
    }

    /* Compute the central binomial coefficient  C(2n+10, 9) */
    seq_t binom = binomial(2*n + 10, (idx_t)9);

    /* Compute 2^(2n-1) */
    seq_t pow = pow2((idx_t)(2*n - 1));

    /* Multiply the two parts */
    return binom * pow;
}
