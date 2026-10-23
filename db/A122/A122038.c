/*
 * A122038  a(n) = 1*3^(3*n) + 2*3^(2*n) - 3*3^(1*n).
 * https://oeis.org/A122038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(3*n) + 2*3^(2*n) - 3^(n+1) (OEIS A122038)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t t1 = pow3(3 * n);      /* 3^(3*n) */
    seq_t t2 = pow3(2 * n);      /* 3^(2*n) */
    seq_t t3 = pow3(n + 1);      /* 3^(n+1) */

    return t1 + 2 * t2 - t3;
}
