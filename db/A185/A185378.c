/*
 * A185378  Number of binary necklaces of 2n beads that are identical when
 *          turned over yet cannot be cut to produce a palindrome.
 * https://oeis.org/A185378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 2^n - 2^((n-2^t)/(2^(t+1))) where t = v₂(n) (number of
 *            * factors of 2 in n) * This implementation directly follows the
 *            conjectured closed form
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   pow2, v2_scalar
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/v2_scalar.h"

seq_t a(idx_t n) {
    /* Compute t = v₂(n) */
    idx_t t = v2_scalar(n);

    /* 2^t as idx_t */
    idx_t two_to_t = ((idx_t)1) << t;

    /* exponent for the second power:
     *   (n - 2^t) / 2^(t+1)
     * The division is exact by definition of t.
     */
    idx_t exp2 = (n - two_to_t) >> (t + 1);

    /* Compute powers of two with arbitrary‑precision support */
    seq_t big_n   = pow2(n);   /* 2^n   */
    seq_t big_exp = pow2(exp2);/* 2^exp2 */

    /* Result */
    return big_n - big_exp;
}
