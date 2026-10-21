/*
 * A227490  The Wiener index of the nanostar dendrimer D_n, defined
 *          pictorially in the Ghorbani et al. references and recursively in
 *          the Deutsch et al. reference.
 * https://oeis.org/A227490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = - 9369 + 56205*2^(n-1) - 75411*2^(2n-2) +
 *            29241*2^(2n-2)*n Implementation uses the explicit formula given
 *            in the OEIS entry. It works for any n >= 1 and for all supported
 *            seq_t types
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 23, u128 n <= 54,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                    /* undefined for offset < 1, harmless */

    /* Compute 2^(n-1) and 2^(2n-2) as seq_t */
    seq_t pow_n_minus_1   = pow2(n - 1);
    idx_t exp_two_n_minus_2 = (n << 1) - 2;   /* 2*n - 2 */
    seq_t pow_2n_minus_2  = pow2(exp_two_n_minus_2);

    /* Terms of the formula */
    seq_t term1 = 56205 * pow_n_minus_1;
    seq_t term2 = 75411 * pow_2n_minus_2;
    seq_t term3 = 29241 * pow_2n_minus_2 * (seq_t)n;

    /* a(n) = -9369 + term1 - term2 + term3
       We compute as (term1 + term3 - term2) - 9369 to avoid
       intermediate negatives when all operands are unsigned. */
    seq_t sum13 = term1 + term3;
    seq_t sub   = sum13 - term2;     /* safe: sum13 >= term2 for n >= 1 */
    seq_t result = sub - 9369;       /* 9369 < sub for all n >= 1 */

    return result;
}
