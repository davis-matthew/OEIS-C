/*
 * A220978  a(n) = 3^(2*n+1) - 3^(n+1) + 1: The left Aurifeuillian factor of
 *          3^(6*n+3) + 1.
 * https://oeis.org/A220978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3^(2*n+1) - 3^(n+1) + 1
 */

#include <stdint.h>
#include "../utils/pow3.h"    /* pow3(idx_t exp) -> seq_t */

seq_t a(idx_t n)
{
    /* compute 3^(2*n+1) */
    idx_t exp1 = (n << 1) + 1;           /* 2*n + 1 */
    seq_t p1 = pow3(exp1);

    /* compute 3^(n+1) */
    idx_t exp2 = n + 1;
    seq_t p2 = pow3(exp2);

    /* a(n) = p1 - p2 + 1 */
    return p1 - p2 + (seq_t)1;
}
