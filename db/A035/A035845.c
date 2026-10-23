/*
 * A035845  Coordination sequence for A_19 lattice.
 * https://oeis.org/A035845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d=1..19} binomial(20,d) * binomial(n-1,d-1) *
 *            binomial(19-d+n,n) (with a(0) = 1)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                 /* a(0) = 1 */

    seq_t sum = 0;
    /* Loop over all possible d (1..19) */
    for (idx_t d = 1; d <= 19; ++d) {
        seq_t term = binomial(20, d);          /* C(20,d)   */
        term *= binomial(n - 1, d - 1);        /* C(n-1,d-1)*/
        term *= binomial(19 - d + n, n);      /* C(19-d+n,n)*/
        sum += term;
    }
    return sum;
}
