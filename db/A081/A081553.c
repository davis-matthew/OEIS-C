/*
 * A081553  Sum of n-th row of A081551.
 * https://oeis.org/A081553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n*10^(n-1) + n*(n-1)/2 (OEIS A081553)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)                /* offset starts at 1, safety for 0 */
        return 0;

    seq_t pow10_n_minus_1 = pow10(n - 1);          /* 10^(n-1)   */
    seq_t term1 = (seq_t)n * pow10_n_minus_1;     /* n * 10^(n-1) */
    seq_t term2 = ((seq_t)n * ((seq_t)n - 1)) / 2;/* n*(n-1)/2  */
    return term1 + term2;
}
