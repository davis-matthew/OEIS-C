/*
 * A212697  a(n) = 2*n*3^(n-1).
 * https://oeis.org/A212697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*3^(n-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* out of defined range – return 0 */

    seq_t p = pow3(n - 1);          /* 3^(n-1) */
    seq_t factor = (seq_t)n * 2;    /* 2*n */
    return p * factor;              /* 2*n*3^(n-1) */
}
