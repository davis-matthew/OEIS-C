/*
 * A116078  Column 0 of triangle A116077.
 * https://oeis.org/A116078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+3)*C(2*n-1,n) - 2^(2*n-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */
    /* bin = C(2*n-1, n) */
    seq_t bin = binom((idx_t)(2*n-1), n);
    /* term1 = (n+3) * bin */
    seq_t term1 = (((seq_t)n) + (seq_t)3) * bin;
    /* term2 = 2^(2*n-1) */
    seq_t term2 = pow2((idx_t)(2*n-1));
    return term1 - term2;
}
