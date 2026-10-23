/*
 * A138233  a(n) = 2^(2*n+1) + 3^(2*n+1).
 * https://oeis.org/A138233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*n+1) + 3^(2*n+1) (OEIS A138233)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* exponent is 2*n + 1 */
    idx_t exp = (n << 1) + 1;

    /* compute 2^exp and 3^exp using the big‑int helper pow_int */
    seq_t term2 = pow_int(2, exp);   /* 2^(2*n+1) */
    seq_t term3 = pow_int(3, exp);   /* 3^(2*n+1) */

    return term2 + term3;
}
