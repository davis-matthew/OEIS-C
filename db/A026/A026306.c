/*
 * A026306  a(n) = T(2n,n+1), where T is the array in A026300.
 * https://oeis.org/A026306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(Sum_{j=0..2*n+1}
 *            binomial(j,-3*n+2*j-1)*binomial(2*n+1,j))/(2*n+1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle n == 0 immediately */
    if (n == 0)
        return (seq_t)0;

    /* sum over j = 0 .. 2*n+1 */
    seq_t sum = 0;
    idx_t two_n_plus_one = 2 * n + 1;
    for (idx_t j = 0; j <= two_n_plus_one; ++j) {
        int64_t k_int = -3 * (int64_t)n + 2 * (int64_t)j - 1; /* -3n+2j-1 */
        if (k_int < 0)
            continue;
        idx_t k = (idx_t)k_int;
        if (k > j)
            continue;

        seq_t term1 = binomial(j, k);
        seq_t term2 = binomial(two_n_plus_one, j);
        sum += term1 * term2;
    }

    seq_t num = (seq_t)n * sum;
    seq_t denom = (seq_t)two_n_plus_one;
    return num / denom;
}
