/*
 * A121285  a(0) = 1; for n>0, a(n) = (n+3)*2^(n-2)-n*binomial(n-1, floor(
 *          (n-1)/2 ))-(n-1)*binomial(n-2,floor((n-2)/2)).
 * https://oeis.org/A121285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+3)*2^(n-2) - n*binomial(n-1, floor((n-1)/2)) -
 *            (n-1)*binomial(n-2, floor((n-2)/2)) with a(0) = 1, a(1) = 1
 *            (formula valid for n ≥ 2)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0 || n == 1) {
        return 1;
    }

    /* Term 1: (n+3) * 2^(n-2) */
    seq_t term1 = pow2(n - 2);
    term1 *= (seq_t)(n + 3);  /* multiply by n+3 */

    /* Term 2: n * binomial(n-1, floor((n-1)/2)) */
    idx_t k1 = (n - 1) / 2;
    seq_t term2 = binomial(n - 1, k1);
    term2 *= (seq_t)n;

    /* Term 3: (n-1) * binomial(n-2, floor((n-2)/2)) */
    idx_t k2 = (n - 2) / 2;
    seq_t term3 = binomial(n - 2, k2);
    term3 *= (seq_t)(n - 1);

    /* Result */
    return term1 - term2 - term3;
}
