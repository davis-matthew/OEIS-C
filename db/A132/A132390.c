/*
 * A132390  Number of binary pattern classes in the (2,n)-rectangular grid;
 *          two patterns are in same class if one of them can be obtained by
 *          reflection or rotation of the other one.
 * https://oeis.org/A132390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(n-1) + 2^(n-2)*(3 + (n mod 2)) (for n ≥ 3); a(1) = 3,
 *            a(2) = 6
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Offset: 1 */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)6;
    /* n ≥ 3 */
    /* term1 = 4^(n-1) = 2^(2*(n-1)) = 2^(2*n-2) */
    idx_t exp1 = 2 * n - 2;
    seq_t term1 = pow2(exp1);
    /* term2 = 2^(n-2) * (3 + (n mod 2)) */
    idx_t exp2 = n - 2;
    seq_t pow2_exp2 = pow2(exp2);
    seq_t factor = (seq_t)(3 + (n & 1u));
    seq_t term2 = pow2_exp2 * factor;
    return term1 + term2;
}
