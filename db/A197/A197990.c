/*
 * A197990  Number of binary arrangements of total n 1's, without adjacent 1's
 *          on n X n torus connected n-s.
 * https://oeis.org/A197990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n*binomial(n^2-n-1, n-1) + n*(-1)^n (n > 1) a(0) = 1,
 *            a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle offsets */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Compute the binomial coefficient */
    idx_t num  = n * n - n - 1;   /* n^2 - n - 1 */
    idx_t denom = n - 1;
    seq_t bin = binomial(num, denom);

    /* Multiply by n: (seq_t)n * bin */
    seq_t result = (seq_t)n * bin;

    /* Add or subtract n according to (-1)^n */
    if ((n & 1) == 0)      /* even n */
        result += (seq_t)n;
    else                    /* odd n */
        result -= (seq_t)n;

    return result;
}
