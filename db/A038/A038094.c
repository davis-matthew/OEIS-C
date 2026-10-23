/*
 * A038094  Number of rooted graphs on n labeled nodes where the root has
 *          degree 2.
 * https://oeis.org/A038094
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*binomial(n-1,2)*2^binomial(n-1,2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;                 /* behaviour below offset not required */

    seq_t bin = binom(n - 1, 2);          /* bin = (n-1 choose 2) */
    idx_t exp = (idx_t)bin;               /* exponent fits in idx_t for n ≤ 10000 */
    seq_t pow_two = pow2(exp);            /* 2^exp */

    return (seq_t)n * bin * pow_two;      /* final product */
}
