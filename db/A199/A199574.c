/*
 * A199574  The number of simple labeled graphs on n nodes where two such
 *          graphs are considered equivalent iff one can be obtained from the
 *          other by reversing the labeling.
 * https://oeis.org/A199574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^C(n,2) + 2^((C(n,2)+floor(n/2))/2)) / 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where C(n,2) = n*(n-1)/2 is the number of edges in a complete graph on n vertices. */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* floor(n/2) */
    idx_t f = n / 2;

    /* C(n,2) = n*(n-1)/2 : fits in idx_t for all n <= 10^4 */
    idx_t m = (idx_t)n * (idx_t)(n - 1) / 2;

    /* exponent for the second power: (m + f) / 2 */
    idx_t exp2 = (m + f) / 2;

    /* 2^m and 2^exp2 using the utility function */
    seq_t term1 = pow_uint(2, m);
    seq_t term2 = pow_uint(2, exp2);

    seq_t sum = term1 + term2;

    /* division by 2 */
    return sum / 2;
}
