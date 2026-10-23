/*
 * A193457  Paradigm shift sequence with procedure length p=5.
 * https://oeis.org/A193457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m^(C-R) * (m+1)^R where C = max(1, floor((n+9)/11)), m =
 *            floor((n+5)/C) - 5, R = (n+5) mod C (OEIS A193457)
 * Range:     u8 n <= 29, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute the value of C.  For n ≥ 1, C can be 0 when n+9 < 11.  */
    idx_t C = (n + 9) / 11;
    if (C < 1) C = 1;

    /* m = floor((n+5)/C) - 5 */
    seq_t m = ((seq_t)(n + 5) / (seq_t)C) - 5;

    /* R = (n+5) mod C */
    idx_t R = (n + 5) % C;

    /* Compute the two powers and multiply them */
    seq_t pow1 = big_pow(m,          (seq_t)(C - R));
    seq_t pow2 = big_pow(m + 1u,     (seq_t)R);

    return pow1 * pow2;
}
