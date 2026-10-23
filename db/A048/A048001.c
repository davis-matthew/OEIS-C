/*
 * A048001  Number of nonempty subsets of {1,2,...,n} in which exactly 1/6 of
 *          the elements are <= n/3.
 * https://oeis.org/A048001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..ceil(2*n/3)/5} binomial(floor(n/3),k) *
 *            binomial(ceil(2*n/3),5*k)
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 39, u64 n <= 75, u128 n <= 145,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* n/3 and ceil(2*n/3) as integer values */
    idx_t n3 = n / 3;                       /* floor(n/3)          */
    idx_t n2 = (2 * n + 2) / 3;             /* ceil(2*n/3)         */
    idx_t kmax = n2 / 5;                    /* floor(n2/5)         */

    seq_t result = 0;
    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t term1 = binomial(n3, k);
        seq_t term2 = binomial(n2, 5 * k);
        result += term1 * term2;
    }
    return result;
}
