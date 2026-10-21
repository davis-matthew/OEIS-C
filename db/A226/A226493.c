/*
 * A226493  Closed walks of length n in K_4 graph.
 * https://oeis.org/A226493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(-1)^n + 3^n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t p = pow3(n);          /* p = 3^n */
    if (n & 1)                  /* n is odd: subtract 3 */
        return p - 3;
    else                        /* n is even: add 3 */
        return p + 3;
}
