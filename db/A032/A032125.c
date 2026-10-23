/*
 * A032125  "BIK" (reversible, indistinct, unlabeled) transform of 3,3,3,3...
 * https://oeis.org/A032125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^(n-2)*(2^(n-1)+1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* out of range, harmless */

    /* Base cases */
    if (n == 1)
        return (seq_t)3;
    if (n == 2)
        return (seq_t)9;

    /* Compute the term via the closed formula */
    seq_t pow_n_2 = pow2(n - 2);          /* 2^(n-2)   */
    seq_t pow_n_1 = pow2(n - 1);          /* 2^(n-1)   */
    return (seq_t)3 * pow_n_2 * (pow_n_1 + (seq_t)1);
}
