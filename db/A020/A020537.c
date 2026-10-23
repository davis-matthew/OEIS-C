/*
 * A020537  a(n) = 4*8^n - 3*2^n.
 * https://oeis.org/A020537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*8^n - 3*2^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    /* compute 8^n as 2^(3n) */
    idx_t three_n = n * 3ULL;
    seq_t pow8 = pow2(three_n);

    /* term1 = 4 * 8^n */
    seq_t term1 = mul_small(pow8, 4);

    /* term2 = 3 * 2^n */
    seq_t pow2n = pow2(n);
    seq_t term2 = mul_small(pow2n, 3);

    return term1 - term2;
}
