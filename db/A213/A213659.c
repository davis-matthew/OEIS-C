/*
 * A213659  a(n) = 3^n + 2^(2*n + 1).
 * https://oeis.org/A213659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + 2^(2*n + 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n) {
    /* 3^n */
    seq_t term3 = pow3(n);
    /* 2^(2*n + 1)  -- compute exponent carefully as idx_t */
    seq_t term2 = pow2(2 * n + 1);
    return term3 + term2;
}
