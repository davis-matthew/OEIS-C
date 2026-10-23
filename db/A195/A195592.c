/*
 * A195592  Number of ways to place 4n nonattacking kings on a vertical
 *          cylinder 8 X 2n.
 * https://oeis.org/A195592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 20, u64 n <= 50, u128 n <= 113,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A195592: Number of ways to place 4n nonattacking kings on a
    vertical cylinder 8×2n.  The closed form from OEIS is
    a(n) = (221*n - 779)*2^n + 44*n^2 + 324*n + 780.
    This function implements that formula using seq_t arithmetic.
*/
#include <stdint.h>
#include "../utils/pow2.h"   /* for seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* cast to seq_t before multiplication to preserve precision */
    seq_t term1 = ((seq_t)221 * (seq_t)n - (seq_t)779) * pow2(n);
    seq_t term2 = (seq_t)44 * (seq_t)n * (seq_t)n;   /* 44*n^2 */
    term2 += (seq_t)324 * (seq_t)n;                  /* +324*n */
    term2 += (seq_t)780;                             /* +780 */
    return term1 + term2;
}
