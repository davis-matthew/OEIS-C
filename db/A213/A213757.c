/*
 * A213757  Principal diagonal of the convolution array A213756.
 * https://oeis.org/A213757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -2 + 2^(n+1) - 6*n + n*2^(n+2) - 3*n^2
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 25, u64 n <= 56, u128 n <= 100,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, guard for completeness */
        return 0;

    seq_t  n_s     = (seq_t)n;
    seq_t  pow1    = pow2((idx_t)(n + 1));
    seq_t  pow2_val = pow2((idx_t)(n + 2));

    seq_t  result = pow1;
    result += n_s * pow2_val;                /* n*2^(n+2) */
    result -= (seq_t)(6 * n);                /* -6*n */
    result -= (seq_t)(3 * n * n);            /* -3*n^2 */
    result -= 2;                             /* -2 */

    return result;
}
